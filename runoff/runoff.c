#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
string preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

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
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);


            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
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
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

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
bool vote(int voter, int rank, string name)
{
    int verify = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp (name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = name;
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

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for ( int i = 0; i < voter_count; i++) //loop para iterar sobre todos os votantes
	{
		int rankpf = 0; //define a preferencia inicial como 0.
		for (int j = 0; j < candidate_count; j++)
		{//loop para buscar o candidato de preferencia 0, sendo J o primeiro candidato
			if(strcmp (preferences[i][rankpf], candidates[j].name) == 0)
			{//se a preferencia 0 do primeiro votante (i) for o primeiro candidato (j)
				if (candidates[j].eliminated == false) //e se este candidato não estiver eliminado
				{
					candidates[j].votes ++; //de a este candidato um voto
                    break; // E parta para o proximo votante completando este loop.
				}
				else //se estiver eliminado, compare agora com a proxima preferencia do votante
				{
					rankpf++; //aumente o rank
                    j = -1; //resete o J para podermos verificar todos os candidatos novamente
				}
			} //se a preferencia 0 do primeiro votante nao for o candidato j,
		}
	}
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    string winner;
    int verify = 0;
    int totalvotes = 0;
    for(int i = 0; i < candidate_count; i++)
    {
        totalvotes = totalvotes + candidates[i].votes;
    }
    for (int i =0; i < candidate_count; i++)
    {
        if (candidates[i].votes > (totalvotes/2))
        {
            verify = 1;
            winner = candidates[i].name;
        }
        else
        {
            verify = 0;
        }
    }
    if (verify == 1)
    {
        printf("%s\n", winner);
        return true;
    }
    else
    {
       return false;
    }
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    //Define the loser
    int loser;
    for(int i = 0, j = 1; j < candidate_count; j++)
    {
        if (candidates[i].eliminated == true)
        {
            i++;
            continue;
        }
        else if (candidates[j].eliminated == true)
        {
            continue;
        }
        if (candidates[i].votes < candidates[j].votes)
        {
            loser = candidates[i].votes;
        }
        else if (candidates[i].votes > candidates[j].votes)
        {
            loser = candidates[j].votes;
            i = j;
        }
        else
        {
                loser = candidates[i].votes;
        }
    }
    return loser;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
	int verify = 0;
	for(int i = 0; i < candidate_count; i ++)
	{
		if ((candidates[i].votes == min) && (candidates[i].eliminated == false))
		{
			verify++;
		}
		else if (candidates[i].eliminated == true)
		{
			verify++;
		}
	}
	if (verify == candidate_count)
	{
		return true;
	}
	else
	{
		return false;
	}

}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for(int i = 0; i < candidate_count; i++)
    {
        if(candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}