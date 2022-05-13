import sys
import csv

candidate_count=0
voter_count=0

# preferences[i][j] is jth preference for voter i
preferences=[]

# Candidates have name, vote count, eliminated status
class Candidate:
  def __init__(self,name,votes,eliminated):
    self.name=name
    self.votes=votes
    self.eliminated=eliminated

# list of candidates
candidates=[]

# Record preference if vote is valid
def vote(voter,rank,name):
    valid=False
    index=0
    for i in range(candidate_count):
          if (name==candidates[i].name):
            valid=True
            index=i
            break
    if valid:
        preferences[voter].append(index)
        return True
    return False

# Tabulate votes for non-eliminated candidates
def tabulate():
        for i in range(voter_count):
          iteration=0;
          while True:
            if not(candidates[preferences[i][iteration]].eliminated):
                  candidates[preferences[i][iteration]].votes+=1
                  break
            iteration+=1

# Print the winner of the election, if there is one
def print_winner():
    for i in range(candidate_count):
        if (candidates[i].votes>voter_count/2):
            print(f"\nWinner:{candidates[i].name}\n")
            return True
    return False


# Return the minimum number of votes any remaining candidate has
def find_min():
    min=2147483647
    for i in range(candidate_count):
        if (not(candidates[i].eliminated) and candidates[i].votes<min):
          min=candidates[i].votes
    return min

# Return true if the election is tied between all candidates, false otherwise
def is_tie(min):
    count=0
    for i in range(candidate_count):
        if candidates[i].votes==min:
          count+=1
    if count==candidate_count:
      return True
    return False

# Eliminate the candidate (or candidates) in last place
def eliminate(min):
    for i in range(len(candidates)):
      if candidates[i].votes==min:
        candidates[i].eliminated=True

def main():
    global candidate_count
    global voter_count
    if __name__=="__main__":
      argc=len(sys.argv)
      # Check for invalid usage
      if (argc< 2):
         print("Usage: runoff [candidate ...]\n")
         return 1

    # Populate array of candidates
    candidate_count = argc - 1

    for i in range(candidate_count):
        leader=Candidate(sys.argv[i + 1],0,False)
        candidates.append(leader)

    voter_count =int(input("Number of voters: "))

    # Keep querying for votes
    for i in range(voter_count):
        preferences.append([])
        # Query for each rank
        for j in range(candidate_count):
          name = input(f"Rank {j+1}: ")

          # Record vote, unless it's invalid
          if not(vote(i, j, name)):
                print("Invalid vote.\n")
                return 4
        print()

    # Keep holding runoffs until winner exists
    while (True):
      # Calculate votes given remaining candidates

      tabulate()

      # Check if election has been won
      won = print_winner()
      if (won):
        break

      # Eliminate last-place candidates
      min = find_min()
      tie = is_tie(min)

      # If tie, everyone wins
      if (tie):
            for i  in range(candidate_count):
                if not(candidates[i].eliminated):
                  print(f"{candidates[i].name}\n")
            break

      # Eliminate anyone with minimum number of votes
      eliminate(min)

      # Reset vote counts back to zero
      for i in range(candidate_count):
        candidates[i].votes = 0
    # Save the ballots as csv file
    with open("election.csv","w") as f:
      a=csv.writer(f)
      title=["VOTERS"]
      for i in range(candidate_count):
        title.append(f"RANK {i}")
      a.writerow(title)
      l=[]
      for i in range(voter_count):
        l2=[f"VOTER{i}"]
        for j in range(candidate_count):
          l2.append(f"{candidates[preferences[i][j]].name}")
        l.append(l2)
      a.writerows(l)

    return 0
    
if __name__=='__main__':
  main()