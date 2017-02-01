import System.Environment

foo :: Integer -> [Integer] -> [Integer] -> Maybe [Integer]
foo   0         _           outpts    = Just outpts
foo   _         []          _         = Nothing
foo   t         (n:nbrs)    outpts    =
  if   n > t
  then foo t nbrs outpts
  else
    case foo (t-n) nbrs (n:outpts) of
      Nothing -> foo t nbrs outpts
      Just ns -> Just ns

main = do
  (filename:_) <- getArgs
  file <- readFile filename
  let nbrs = map read $ lines file
  let target = (foldl (+) (0::Integer) nbrs) `div` 2
  let soln = foo target nbrs []
  putStrLn $ "Size: " ++ (show $ length nbrs) ++ "\n" ++ "Target: " ++ (show target) ++ "\n" ++ (show soln)