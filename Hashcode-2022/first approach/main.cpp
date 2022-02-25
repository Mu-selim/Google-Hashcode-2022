#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <string>
#define ll long long int
using namespace std;

typedef map<string, ll> skills_;
typedef map<ll, ll> skillValue;
typedef vector<pair<ll, ll>> skillRequired;

void fast()
{
    cin.tie(0);
    cin.sync_with_stdio(0);
}
void readFile()
{
    freopen("f_find_great_mentors.in.txt", "r", stdin);
}
void writeFile()
{
    freopen("f.out.txt", "w", stdout);
}

struct Project
{
    string name; ll days, score, roles;
    skillRequired skill;
};

struct Counter
{
    string name; skillValue skill;
};

struct Result
{
    string name; list<string> a;
};

// Global variables that will be used
skills_ skills;
ll skills_count;
vector<Counter> counter;
vector<Project> project;
list<Result> sol;

int skillKey(const string & s)
{
    auto i = skills.find(s);
    if (i == skills.end())
    {
        auto j = skills.insert({s, skills_count++});
        i = j.first;
    }
    return i->second;
}

void counterRead(Counter & c)
{
    ll n; cin >> c.name >> n;
    for (ll i = 0 ; i < n ; ++i)
    {
        string s; ll sk;
        cin >> s >> sk;
        c.skill[skillKey(s)] = sk;
    }
}

void projectRead(Project &p)
{
    ll n; cin >> p.name >> p.days >> p.score >> p.roles >> n;
    for (ll i = 0 ; i < n ; ++i)
    {
        string s; ll sk; cin >> s >> sk;
        p.skill.emplace_back(skillKey(s), sk);
    }
}

bool projectTry(Project &p, Result &r)
{
    vector<bool> used(counter.size());
    for (auto s : p.skill)
    {
        bool found = false;
        for (ll c = 0 ; c < counter.size() ; ++c)
        {
            if (used[c])
                continue;

            if (counter[c].skill[s.first] >= s.second)
            {
                used[c] = true;
                r.a.emplace_back(counter[c].name);
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}

void Solution()
{
    for (Project &p : project)
    {
        Result r;
        if (projectTry(p, r))
        {
            r.name = p.name;
            sol.emplace_back(r);
        }
    }
}

void Result()
{
    cout << sol.size() << "\n";
    for (auto r : sol)
    {
        cout << r.name << "\n";
        for (auto a : r.a)
            cout << a << " ";
        cout << "\n";
    }
}

int main()
{
    fast();
    // read and write functions;
    readFile(); writeFile();

    int cc, pp; cin >> cc >> pp;
    counter.resize(cc);
    for (Counter & c : counter)
        counterRead(c);
    project.resize(pp);
    for (Project &p : project)
        projectRead(p);
    Solution();
    Result();
}