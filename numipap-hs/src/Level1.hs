module Level1 where

import           Data.Char  (isDigit, isSpace)
import           Data.Fixed (mod')
import           Data.List  (foldl', intercalate, span)

main :: IO ()
main = do
  putStrLn "Enter some expression:"
  source <- getLine
  putStrLn ""
  mapM_ print $ interpret source

type OperandStack = [Node]

type FunctionSet = [(String, OperandStack -> OperandStack)]

data Interpreter =
  Interpreter
    { iStack    :: OperandStack -- instruction stack
    , eStack    :: OperandStack -- execution stack
    , functions :: FunctionSet -- available functions
    }

data Node
  = FunctionCall String
  | NumberLiteral Double
  | Error String

instance Show Node where
  show (FunctionCall n)  = n
  show (NumberLiteral x) = show x
  show (Error e)         = e

interpret :: String -> OperandStack
interpret s = evaluate interp
  where
    interp =
      Interpreter
        { iStack = instructionStack
        , eStack = []
        , functions =
            [ ("+", lNB (+))
            , ("-", lNB (-))
            , ("*", lNB (*))
            , ("/", lNB (/))
            , ("mod", lNB mod')
            , ("sin", lNU sin)
            , ("cos", lNU cos)
            , ("tan", lNU tan)
            ]
        }
    instructionStack = parseAll s

parseAll :: String -> OperandStack
parseAll [] = []
parseAll s@(c:cs)
  | isSpace c = parseAll cs
  | otherwise = node : parseAll rest
  where
    (node, rest) = parse s

type ParseResult = (Node, String)

parse :: String -> ParseResult
parse str@(c:cs)
  | isDigit c = parseNumber str
  | isSpace c = parse cs
  | otherwise = parseFunction str
parse str = (Error str, "")

-- general parsing function
parseG ::
     (String -> Node) -- node constructor
  -> (Char -> Bool) -- stop condition
  -> (String -> String) -- rest filter
  -> String
  -> ParseResult
parseG cons stop fltr str = (cons ini, fltr rest)
  where
    (ini, rest) = span stop str

-- apply two functions to a value and a function to the two results
-- e.g. apply2way (||) isDigit isSpace is equal to \c -> isDigit c || isSpace c
apply2way :: (b -> c -> d) -> (a -> b) -> (a -> c) -> a -> d
apply2way f g h a = f (g a) (h a)

parseNumber, parseFunction :: String -> ParseResult
parseNumber = parseG (NumberLiteral . read) (apply2way (||) isDigit (== '.')) id

parseFunction = parseG FunctionCall (not . isSpace) id

-- Lift numeric binary operation to the stack
lNB :: (Double -> Double -> Double) -> OperandStack -> OperandStack
lNB f = g
  where
    g [] = error "Not enough arguments in binary function"
    g ((NumberLiteral i1):(NumberLiteral i2):is) = NumberLiteral (f i1 i2) : is
    g _ = error "Wrong arguments for binary function"

lNU :: (Double -> Double) -> OperandStack -> OperandStack
lNU f = g
  where
    g []                      = error "Not enough arguments in unary function"
    g ((NumberLiteral i1):is) = NumberLiteral (f i1) : is
    g _                       = error "Wrong arguments for unary function"

evaluate :: Interpreter -> OperandStack
evaluate int@Interpreter {iStack = iStack} = eStack $ foldl' g int iStack
  where
    g :: Interpreter -> Node -> Interpreter
    g int i@(NumberLiteral _) = int {eStack = i : eStack int}
    g int@Interpreter {functions = fs, eStack = es} (FunctionCall f) =
      int
        { eStack =
            case lookup f fs of
              Just g  -> g es
              Nothing -> error $ "Attempted to call unknown function: " ++ f
        }
    g int (Error _) = int
