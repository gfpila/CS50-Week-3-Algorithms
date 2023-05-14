#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

// Verify int
int verify = 0;

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }
    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");
        vote (name);

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes ++;
            verify++;
        }
    }
    if (verify == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    //Define the biggest voter
    int winner;
    for(int i = 0, j = 1; j < candidate_count; j++)
    {
        if (candidates[i].votes > candidates[j].votes)
        {
            winner = candidates[i].votes;
        }
        else if (candidates[i].votes < candidates[j].votes)
        {
            winner = candidates[j].votes;
            i = j;
        }
        else
        {
            winner = candidates[i].votes;
        }
    }
    //Check wich candidate has this number of votes
    string winners[MAX];
    int nw = 0;
    for (int i = 0, j=0; i < candidate_count; i++)
    {
        if(candidates[i].votes == winner)
        {
            winners[j] = candidates[i].name;
            j++;
            nw++;
        }
    }
    for(int i = 0; i < nw; i++)
    {
        printf("%s\n", winners[i]);
    }
    return;
}