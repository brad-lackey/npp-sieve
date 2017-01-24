import System.Random
import System.Environment
import qualified Data.Set as Set

getNumbers :: (Ord t, Random t) => (t,t) -> Int -> Set.Set t -> IO (Set.Set t)
getNumbers rg n st = do
  if (n == Set.size st )
    then return st
    else do
      nbr <- randomRIO rg 
      let st' = Set.insert nbr st
      getNumbers rg n st'

main = do
  (arg0:arg1:_) <- getArgs
  let num_nbr = read arg0 :: Int
  let max_nbr = (2 :: Integer) ^ (read arg1 :: Integer)
--  putStrLn $ "Number of numbers: " ++ (show num_nbr) ++ ". Size bound: " ++ (show max_nbr)
  st <- getNumbers (1::Integer, max_nbr) num_nbr Set.empty
  putStr $ foldl (\str nb -> str ++ (show nb) ++ "\n") "" (reverse $ Set.toList st)
  