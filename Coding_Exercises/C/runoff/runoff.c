#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate; // same struct as in plural except we're dealing with bool eliminated for elminations in our code

// Array of candidates
candidate candidates[MAX_CANDIDATES]; //an array of candidates like last time once its listed 

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min); // these are the six functions that we need to complete 

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2; 
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    } 

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    } // from line 37 to this line, its the same as plural 

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++) // here is where the code starts to differientate itself from plurality 
    {

        // Query for each rank, 
        for (int j = 0; j < candidate_count; j++) // this line makes it so the vote can vote for as many candidates there are
        {                                         // in a list. For ex) 4 candidates, voter is like 1. jon 2. pop 3. junior 4. alex 
            string name = get_string("Rank %i: ", j + 1); // the j+1 is for the loop of this for loop 

            // Record vote, unless it's invalid
            if (!vote(i, j, name)) // this a little confusing to grasp as to why i,j are here but similar to plural if name does exist 
            {                      // , then the vote is invalid 
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate(); //function we're gonna do later on

        // Check if election has been won
        bool won = print_winner(); // function we're gonna later on 
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min(); // function we're gonna do later on
        bool tie = is_tie(min); // function we're gonna do later on 

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name) // we start by doing vote which affects line 77's code 
{
    for (int i=0; i<candidate_count; i++) // is the obviously the candidate just like from plurality, i'm going through each candidate argv[] 
        {
            if (strcmp(candidates[i].name, name) == 0) //instead of plurality, one person gets to vote for many candidates they are
                {                                      // for example, if there's 3 candidates, the voter gets to vote 3 times 
                                                       // this checks each name and sees if there is a match 
                    preferences[voter][rank] = i; //
                    return true;  
                    printf("%d\n", i);// what i'm trying to do here is make a prefence table out of my candidate table
                }                                         

        }
    return false;
}
// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int candidate_number;
     for(int i=0; i<voter_count; i++) // every voter
    {
        for(int j=0; i<candidate_count; j++) // every candidate, these two loops allows us to reference the preference table again
        {
            candidate_number=preferences[i][j]; // we could make pref = candidates[i].name instead but by adding a variable it gets 
                                                // rid of another if statement. 
            // test_candidate.elminated meaning if the candidate hasn't been eliminated
            if (candidates[candidate_number].eliminated == false)
            {
                candidates[candidate_number].votes++;
                break; // break cancels the j loop, so we don't count anymore people in a voter's list 
            }
            
           
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int target = round(voter_count/2); // the winner needs to win by majority. Ex) if there is 7 voters then the winner must have 
    for (int i = 0; i<candidate_count; i++) // at least 4 votes. 
    {                                       // we go through every candidate as usual, we don't care about the voters anymore 
        if (candidates[i].votes > target)   // after we're fnished counting their votes. 
        {
            printf("%s\n" , candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
  int min_votes = MAX_VOTERS; 
    for(int i=0; i<candidate_count; i++) // this like the opposite of trying to finder the winner in plurality 
        {                                // in order for me to figure it out I make min_votes a really higher number or... the max amount of voters
        if ( min_votes > candidates[i].votes && candidates[i].eliminated == false)
            { //lets say there's 7 voters and tommy only gets one vote, then min_votes changes from 7 to 1 and then loops again
                min_votes = candidates[i].votes; //if a candidate[i].votes shows up with anything higher than 1, then its ignored
            }                                   // because the if statement 1>2 will be false, and that's how we figure out the minimum
        }                                       // number of votes
return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    for(int i = 0; i < candidate_count; i++) // i'm going through every candidate to weed out the ones who are elminated through a min value
    {
        if (!candidates[i].eliminated && candidates[i].votes != min) //if the candidates does not have a minmum value and they haven't been 
        {                                                            // elminated, then they are still in the running 
            return false; // false means that they are still in the running
        }
   
    }
return true; // true means that they are out
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for(int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true; // this is kind of like the opposite of the last function, if they do have the min votes
        }                                    // then they are out!

    }

    return; 
}


