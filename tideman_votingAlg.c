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

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void bubbleSort();
void swap(pair *xp, pair *yp);
bool createsCycle(int i, int j);
bool tempcycle = false;
bool checkCycle(int index);
bool checkCycleHelper(int index, bool visited[]);

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
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
        for (int j = 2 - i; j > 0; j--)
        {
            preferences[ranks[i]][ranks[candidate_count - j]] += 1;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int cand = candidate_count - 1;

    int pairsPerVote = 0;
    for (int i = 1; i < candidate_count; i++)
    {
        for (int j = 0; j < cand; j++)
        {
            if (j != i) 
            {
                pair pairOne;
                int a_vs_b = preferences[j][i];
                int b_vs_a = preferences[i][j];

                if (a_vs_b > b_vs_a)
                {
                    pairOne.winner = j;
                    pairOne.loser = i;
                }

                if (b_vs_a > a_vs_b)
                {
                    pairOne.winner = i;
                    pairOne.loser = j;
                }

                if (b_vs_a != a_vs_b)
                {
                    pairs[pairsPerVote] = pairOne;
                    pairsPerVote++;
                    pair_count++;
                }
            }
        }
    }

    return;
}

void bubbleSort()
{
    int n = candidate_count;
    int i, j;
    bool swapped;

    for (i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
        {
            //printf("winner: %d\n", pairs[j].winner);
            //printf("loser: %d\n", pairs[j].loser);
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                swap(&pairs[j], &pairs[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
        {
            break;
        }
    }
}

void swap(pair *xp, pair *yp)
{
    pair temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    bubbleSort();
    return;
}

void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true; //add the pair to locked array
        
        if (checkCycle(pairs[i].winner))

        {
            locked[pairs[i].winner][pairs[i].loser] = false; //remove the pair from locked array
        }
    }
}

bool checkCycle(int index)
{
    bool visited[candidate_count];
    
    for (int i = 0; i < candidate_count; i++)
    {
        visited[i] = false;
        //fill our new array with falses
    }
    return checkCycleHelper(index, visited);
}

bool checkCycleHelper(int index, bool visited[])
{
    if (visited[index])
    {
        return true;
    }
    visited[index] = true;
    
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[index][i] && checkCycleHelper(i, visited))
            //   if (locked[index][i] && checkCycleHelper(pairs[i].loser, visited))
        {
            return true;
        }
    }
    return false;
}

void print_winner(void)
{
    for (int cand = 0; cand < candidate_count; cand++)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][cand] == true)
            {
                //printf("winner is not %s\n", candidates[cand]);
                break;
            }
            if ((locked[i][cand] == false) && i == candidate_count - 1)
            {
                //printf("winner is %s\n", candidates[cand]);
                printf("%s\n", candidates[cand]);
            }
        }
        
    }
}