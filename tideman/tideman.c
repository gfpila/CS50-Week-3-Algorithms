#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int unlocked;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for(int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i])== 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences [i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int save;
    pair saved;
    int index;
    int strenght [pair_count];
    for(int i = 0; i < pair_count; i++) //this will define the strenght array
    {
    strenght[i] = preferences[(pairs[i].winner)][(pairs[i].loser)]
    - preferences[(pairs[i].loser)][(pairs[i].winner)];
    }

    for (int b = 0; b < pair_count; b ++)
    {
        index = b;
        for (int i = 0, j = 1; j < pair_count; j++) // find the biggest value of strenght
        {
            if(strenght[j] > strenght[i])
            {
                index = j;
            }
        }
        save = strenght[b];
        strenght[b] = strenght[index];
        strenght[index] = save;

        saved = pairs[b];
        pairs[b] = pairs[index];
        pairs [index] = saved;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for(int i =0; i < pair_count; i++) //lock everyone
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
    }
    int wins[candidate_count];
    int winnerscount = 0;
    for (int i = 0; i < pair_count; i++) //get array wins
    {
        int winner = pairs[i].winner;
        wins[winner]++;
    }
    int saved = wins[0];
    for (int i = 0, j = 1; j < candidate_count; j++) //this will find the biggest win
        {
            if(wins[j] > wins[i])
            {
                i = j;
                saved = wins[j];
            }

        }
    int winners [candidate_count]; //this will find the ones that have most arrows
    for (int i = 0; i < candidate_count; i++)
    {
        if (wins[i] == saved)
        {
            winners[winnerscount] = i;
            winnerscount++;
        }
    }
    if (winnerscount == 1)
    {
        unlocked = winners[0];
    }
    else
    {
        for (int i = pair_count - 1; i >= 0; i--)
        {
            for (int j = 0; j < winnerscount; j++)
            {
                if(winners[j] != pairs[i].loser)
                {
                    continue;
                }
                for (int k = 0; k < winnerscount; k++)
                {
                    if (winners[k] == pairs[i].winner)
                    {
                        unlocked = winners[j];
                        return;
                    }
                }

            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i =0; i < candidate_count; i++) //unlocko este primeiro winner de todos
    {
            locked[i][unlocked] = false;
    }
    printf("%s\n", candidates[unlocked]);
    return;
}