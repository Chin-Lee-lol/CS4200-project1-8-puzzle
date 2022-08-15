#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <queue>
#include <list>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define MAXN 100005
#define L(n) (2*n)
#define R(n) (2*n+1)
#define P(n) (int)(n/2)

struct Node {
    int p;
    string s;
};

template<typename Key, typename Priority>
struct PriorityQueue {
private:
    vector<pair<Key, Priority> > pq;
    int size;

    void bubble_up(int u) {
        while (P(u)) {
            if (pq[P(u)].second > pq[u].second) {
                swap(pq[u], pq[P(u)]);
                u = P(u);
            }
            else break;
        }
    }

    void bubble_down(int u) {
        while (L(u) <= size) {
            int pp = min(pq[u].second, pq[L(u)].second);
            if (R(u) <= size) pp = min(pp, pq[R(u)].second);
            if (pp == pq[u].second) break;
            else if (pp == pq[L(u)].second) {
                swap(pq[u], pq[L(u)]);
                u = L(u);
            }
            else {
                swap(pq[u], pq[R(u)]);
                u = R(u);
            }
        }
    }

public:
    PriorityQueue() : pq(1), size(0) {}

    void add(Key key, Priority priority) {
        pq.push_back(make_pair(key, priority));
        bubble_up(++size);
    }

    Key remove() {
        Key ret = pq[1].first;
        swap(pq[1], pq[size--]);
        pq.pop_back();
        bubble_down(1);
        return ret;
    }
};
// to be honest i just copy this part for priority queue to accept string lol and adjust it so queue from low to high
// for h1 and h2 + depth to be priority
struct node
{
    string input;
    int depth;
};
void solve(string input,int h, int d);
string goal = "012345678";
map<string, bool> explored1, explored2;
map<string, string> parent;
int count1 = 0, count2 = 0, depth = 0; // counter for h1 and h2
PriorityQueue<node, int> Q1; //frontier for h1 and h2
PriorityQueue<node, int> Q2; //frontier for h1 and h2

bool evenInverse(string input) // a function to check for inverse in the string
{
    int inverse = 0;
    for (int i = 0; i < input.length(); i++)
        for (int j = i; j < input.length(); j++)
            if (input.at(j) != '0') // no care 0 (aww poor 0)
                if (input.at(i) > input.at(j))
                    inverse++;
    return (inverse % 2 == 0) ? true : false;
}

string generateMatrix() // generate random matrix with even number of inverse
{
    string lol = "012345678";
    bool nope = true;
    while (nope)
    {
        random_shuffle(lol.begin(), lol.end()); // just shuffle until it good lol
        if (evenInverse(lol))
            nope = false;
    }
    explored1.insert(pair<string, bool>(lol, false)); // throw it in frontier 
    explored2.insert(pair<string, bool>(lol, false));
    return lol;
}

void print(string input) // well obviously lol
{
    cout << endl;
    for (int i = 0; i < 9; i++)
    {
        cout << input.at(i) << ' ';
        if (i % 3 == 2)
            cout << endl;
    }
    cout << endl;
}

bool isGoal(string input) // another obvious function XD
{
    if (input.compare(goal) == 0)
        return true;
    return false;
}

int h1(string input) // simply check correct position
{
    int count = 0;
    for (int i = 0; i < 9; i++)
        if (input.at(i) != goal.at(i))
            count++;
    return count;
}

int h2(string input) // check manhattan spot wow so cool name I like it
{
    int pos = 1; //to help determine postion
    int dist = 0;
    int goalCol[] = { 0,1,2,0,1,2,0,1,2 }; // postion of row and column for goal matrix
    int goalRow[] = { 0,0,0,1,1,1,2,2,2 };
    // idea; /3 = row %3 = column
    for (int i = 1; i < 9; i++) // start from 1 cuz we dont look at 0 (poor 0 D:)
    {
        int cur = input.find('0' + i);
        dist += abs((cur/3 - goalRow[pos])) + abs((cur % 3 - goalCol[pos]));
        pos++;
    }
    return dist;
}

void addFrontier(string input, int pos1, int pos2, int h, int d)
{
    int val1 = 0, val2 = 0, hval = 0;
    string og = input; // for parent
    swap(input[pos1], input[pos2]);
    
    if (h == 1)
    {
        if (explored1.find(input) != explored1.end()) // dont add to frontier if already in there;
            return;        
        val1 = h1(input) + d; // h1 value + depth for priority
        node jk;
        jk.input = input;
        jk.depth = ++d;
        Q1.add(jk, val1); // add to frontier
    }
    else // do the same as h1 but for h2
    {
        hval = h2(input);
        if (explored2.find(input) != explored2.end())    
            return;
        val2 = hval + d;
        node jk;
        jk.input = input;
        jk.depth = ++d;
        parent.insert(pair<string, string>(input, og));
        Q2.add(jk, val2);
        
    }
  
}

void neighbor(string input, int pos, int h, int d)
{
    switch (pos) // add frontier according to position and values
    {
        case 0: addFrontier(input, 0, 1, h, d); addFrontier(input, 0, 3, h, d); break;
        case 1: addFrontier(input, 1, 0, h, d); addFrontier(input, 1, 2, h, d); addFrontier(input, 1, 4, h, d); break;
        case 2: addFrontier(input, 2, 1, h, d); addFrontier(input, 2, 5, h, d); break;
        case 3: addFrontier(input, 3, 0, h, d); addFrontier(input, 3, 4, h, d); addFrontier(input, 3, 6, h, d); break;
        case 4: addFrontier(input, 4, 1, h, d); addFrontier(input, 4, 3, h, d); addFrontier(input, 4, 5, h, d);
            addFrontier(input, 4, 7, h, d); break;
        case 5: addFrontier(input, 5, 2, h, d); addFrontier(input, 5, 4, h, d); addFrontier(input, 5, 8, h, d); break;
        case 6: addFrontier(input, 6, 3, h, d); addFrontier(input, 6, 7, h, d); break;
        case 7: addFrontier(input, 7, 6, h, d); addFrontier(input, 7, 8, h, d); addFrontier(input, 7, 4, h, d); break;
        case 8: addFrontier(input, 8, 7, h, d); addFrontier(input, 8, 5, h, d); break;
    }
    if (h == 1) // grab lowest value string from frontier to visit next
    {
        try
        {
            node jk = Q1.remove();
            string next1 = jk.input; 
            explored1.insert(pair<string, bool>(next1, false));
            solve(next1, 1, jk.depth);
        }
        catch (const std::exception&)
        {
            return;
        }   
    }
    else
    {
        node jk = Q2.remove();
        string next2 = jk.input;
        explored2.insert(pair<string, bool>(next2, false));
        solve(next2, 2, jk.depth);
    }
}

void solve(string input, int h, int d)  // aka explore function
{   
    //print(input);
    if (isGoal(input)) // if goal then done
        return; 
    if (h == 1) // to just use for h1 and h2 in same function
    {
        count1++;
        explored1.insert(pair<string, bool>(input, true)); // mark explored
        int pos = input.find('0');
        neighbor(input, pos, 1, d);
        explored1.erase(input);
    }
    else
    {
        count2++;
        explored2.insert(pair<string, bool>(input, true)); // mark explored
        int pos = input.find('0');
        neighbor(input, pos, 2, d);
        explored2.erase(input);
    }
}

string shuffle(int n) // for specific depth random
{
    string input = goal, og = goal;
    int i = 0;
    explored2.insert(pair<string, bool>(input, true));
    while (true)
    {
        
        input = og;
        int pos = input.find('0');
        int r2 = rand() % 2 + 1, r3 = rand() % 3 + 1, r4 = rand() % 4 + 1;    
        switch (pos) // add frontier according to position and values
        {
        case 0: if (r2 == 1)
            swap(input[0], input[1]);
              else
            swap(input[0], input[3]);
            break;
        case 1: if (r3 == 1)
            swap(input[1], input[0]);
              else if (r3 == 2)
            swap(input[1], input[2]);
              else
            swap(input[1], input[4]);
            break;
        case 2: if (r2 == 1)
            swap(input[2], input[1]);
              else
            swap(input[2], input[5]);
            break;
        case 3:if (r3 == 1)
            swap(input[3], input[0]);
              else if (r3 == 2)
            swap(input[3], input[4]);
              else
            swap(input[3], input[6]);
            break;
        case 4: if (r4 == 1)
            swap(input[4], input[1]);
              else if (r4 == 2)
            swap(input[4], input[3]);
              else if (r4 == 3)
            swap(input[4], input[5]);
              else
            swap(input[4], input[7]);
            break;
        case 5: if (r3 == 1)
            swap(input[5], input[2]);
              else if (r3 == 2)
            swap(input[5], input[4]);
              else
            swap(input[5], input[8]);
            break;
        case 6: if (r2 == 1)
            swap(input[6], input[7]);
              else
            swap(input[6], input[3]);
            break;
        case 7: if (r3 == 1)
            swap(input[7], input[6]);
              else if (r3 == 2)
            swap(input[7], input[4]);
              else
            swap(input[7], input[8]);
            break;
        case 8: if (r2 == 1)
            swap(input[8], input[7]);
              else
            swap(input[8], input[5]);
            break;
        }
        if (explored2.find(input) == explored2.end()) // if not same then find next one
        {
            og = input;
            explored2.insert(pair<string, bool>(input, true));
            i++;
            if (i == n)
            {
                break; break;
            }
        }

    }
    return input;
}

int main()
{
    vector<string> result;
    cout << "Welcome to Chin's 8-puzzle solver\n";
    count1 = 0; count2 = 0;
    int counter = 1, n = 1, run = 1; // <= change run for how many iteration u want
    srand(time(0));
    string input, input2, input3;
    string choice;
    cout << "Options: \n[1] depth randomized matrix\n[2] custom matrix\n[3] true random\ninput: ";
    cin >> choice;
    getline(cin, input);
    if (choice.compare("2") == 0)
    {
        bool cool = true;
        while (cool)
        {
            cout << "\ninput matrix: (Just for your copy paste professor lol)\n";
            getline(cin, input);
            getline(cin, input2);
            getline(cin, input3);
            input = input + input2 + input3;
            input.erase(remove(input.begin(), input.end(), ' '), input.end());
            if (evenInverse(input))
            {
                cool = false;
                explored1.insert(pair<string, bool>(input, false));
                explored2.insert(pair<string, bool>(input, false));
            }
            else
                cout << "This matrix cannot be solve, please input new matrix.\n";
        }
    }
    else if (choice.compare("1") == 0)
    {
        //input = generateMatrix();'
        cout << "What is your desired depth (2-14/15) for h1 or (2-20) for h2 [because longer than that my h1 wont work lol]\n";
        cin >> n;
    }
    else
        input = generateMatrix();
    //cout << "\nYour starting puzzle: \n"; // i mean this hello
    auto start = high_resolution_clock::now();
    for (int i = 0; i < run; i++)
    {
        if (choice.compare("1") == 0) // generate random depth specified matrix
            input = shuffle(n);
        //print(input); // include this line to see starting puzzle and 6 line above this too lol
        explored2.clear();

        //use solve with 2 to find the path from start to solution because solve 1 with h1 cant handle
        //dealing with map parent and I completely dont know how to deal with it, comment it out 
        //or include it to see h1 or h2 result.
        
        solve(input,1, 0);
        solve(input, 2, 0);
        //comment out this part to get rid of printing path to goal <===============
        //result.push_back(goal);
        //string yeha = parent.at(goal);
        //while (true) // get path from goal to initial matrix
        //{
        //    try
        //    {
        //        result.push_back(yeha);
        //        yeha = parent.at(yeha);
        //    }
        //    catch (const std::exception&)
        //    {
        //        break;
        //    }
        //}
        //reverse(result.begin(), result.end());
        //counter = 1;
        //for (int i = 1; i < result.size(); i++) // print from start to goal
        //{
        //    cout << "step " << counter << ":\n";
        //    print(result.at(i));
        //    counter++;
        //}
        // to this part <+++++++++++++++++++++++++++
        parent.clear();
        explored1.clear();
        explored2.clear();
        result.clear();    
       
        cout << "h1 expanded nodes : " << count1 << endl;
        cout << "h2 expanded nodes : " << count2 << endl;
        //cout << "+ " << count1; // for 100 run calculator purpose
        count1 = 0;
        count2 = 0;
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "\nduration: " << ((float)duration.count())/1000000 << endl;
    cout << endl;
    system("pause");
  
    return 0;
}
