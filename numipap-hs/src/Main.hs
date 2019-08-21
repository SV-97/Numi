module Main where

import           Data.Char  (isDigit, isSpace)
import           Data.Fixed (mod')
import           Data.List  (intersperse, span)
import           Data.List  (intercalate)

main :: IO ()
main = do
  putStrLn "Enter some expression:"
  source <- getLine
  putStrLn ""
  mapM_ print $ interpret source

interpret :: String -> OperandStack
interpret s = evaluate interp
  where
    interp = defaultInterpreter {iStack = instructionStack}
    instructionStack = parseAll s

type OperandStack = [Node]

type StackFunc = OperandStack -> OperandStack

type FunctionSet = [(String, StackFunc)]

data Interpreter =
  Interpreter
    { iStack    :: OperandStack -- instruction stack
    , eStack    :: OperandStack -- execution stack
    , functions :: FunctionSet -- available functions
    }

defaultInterpreter =
  Interpreter
    { iStack = []
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
        , ("call", call)
        ]
    }

instance Show Interpreter where
  show i = show (iStack i) ++ show (eStack i)

data Node
  = FunctionCall String
  | NumberLiteral Double
  | Error String
  | Frame OperandStack

instance Show Node where
  show (FunctionCall n)  = n
  show (NumberLiteral x) = show x
  show (Error e)         = e
  show (Frame f)         = "[" ++ (intercalate " " $ map show f) ++ "]"

-- Lift numeric binary operation
lNB :: (Double -> Double -> Double) -> StackFunc
lNB f = g
  where
    g [] = error "Not enough arguments in binary function"
    g ((NumberLiteral i1):(NumberLiteral i2):is) = NumberLiteral (f i1 i2) : is
    g _ = error "Wrong arguments for binary function"

lNU :: (Double -> Double) -> StackFunc
lNU f = g
  where
    g []                      = error "Not enough arguments in unary function"
    g ((NumberLiteral i1):is) = NumberLiteral (f i1) : is
    g _                       = error "Wrong arguments for unary function"

parseAll :: String -> OperandStack
parseAll [] = []
parseAll s@(c:cs)
  | isSpace c = parseAll cs
  | otherwise = node : parseAll rest
  where
    (node, rest) = parse s

type ParseResult = (Node, String)

parse :: String -> ParseResult
parse str@('[':_) = parseFrame str
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

parseNumber, parseFunction, parseFrame :: String -> ParseResult
parseNumber = parseG (NumberLiteral . read) (apply2way (||) isDigit (== '.')) id

parseFunction = parseG FunctionCall (not . isSpace) id

parseFrame = (parseG (Frame . parseAll) (/= ']') (drop 1)) . (drop 1)

evaluate :: Interpreter -> OperandStack
evaluate Interpreter {iStack = [], eStack = eStack} = eStack
evaluate int@Interpreter {iStack = (i:is), eStack = eStack, functions = fSet} =
  case i of
    Frame _ -> evaluate int {iStack = is, eStack = i : eStack}
    NumberLiteral _ -> evaluate int {iStack = is, eStack = i : eStack}
    FunctionCall f ->
      evaluate
        int
          { iStack = is
          , eStack =
              case lookup f fSet of
                Just g -> g eStack
                Nothing -> error $ "Attempted to call unknown function: " ++ f
          }
    Error e -> evaluate int {iStack = is}

call :: StackFunc
call ((Frame fStack):es) =
  evaluate defaultInterpreter {iStack = fStack, eStack = es}
