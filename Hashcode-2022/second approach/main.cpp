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
    freopen("d_dense_schedule.in.txt", "r", stdin);
}
void writeFile()
{
    freopen("d.out.txt", "w", stdout);
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
    string name; list<Counter *> a;
};

// Global variables that will be used
skills_ skills;
ll skills_count;
vector<Counter> counter;
vector<Project> project;
list<Result> sol;

bool compareSkill(const Project &p1, const Project &p2)
{
    return (p1.roles < p2.roles);
}

int skillKey(const std::string &s)
{
    auto i = skills.find(s);
    if (i == skills.end())
    {
        auto j = skills.emplace(s, skills_count++);
        i = j.first;
    }
    return i->second;
}

void counterRead(Counter &c)
{
    ll n; cin >> c.name >> n;
    for (int i = 0 ; i < n ; ++i)
    {
        string s; ll sk; cin >> s >> sk;
        c.skill[skillKey(s)] = sk;
    }
}

void projectRead(Project &p)
{
    ll n; cin >> p.name >> p.days >> p.score >> p.roles >> n;
    for (ll i = 0 ; i < n ; ++i)
    {
        string s; ll sk;
        cin >> s >> sk;
        p.skill.emplace_back(skillKey(s), sk);
    }
}

bool projectTry(Project &p, Result &r)
{
    bool mentors = false;
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
                r.a.emplace_back(&counter[c]);
                found = true;
                break;
            }
        }
        if (!found)
        {
            r.a.push_back(NULL);
            mentors = true;
        }
    }
    if (mentors)
    {
        auto a = r.a.begin();
        for (auto s : p.skill)
        {
            if (!*a)
            {
                for (auto m : r.a)
                {
                    if (m && m->skill[s.first] >= s.second)
                    {
                        for (int c = 0 ; c < counter.size() ; ++c)
                        {
                            if (used[c])
                                continue;
                            if (counter[c].skill[s.first] == s.second - 1)
                            {
                                used[c] = true;
                                *a = &counter[c];
                                break;
                            }
                        }
                    }
                }
            }
            if (!*a)
                return false;
            ++a;
        }
    }
    return true;
}

void Solution()
{
    ll curtime = 0, count = 0;
    for (Project &p : project)
    {
        cout << "Trying project " << count++ << "\n";
        if (curtime + p.days >= p.roles + p.score)
            continue;
        Result r;
        if (projectTry(p, r))
        {
            curtime += p.days;
            r.name = p.name;
            sol.emplace_back(r);
            auto a = r.a.begin();
            for (auto s : p.skill)
            {
                if ((*a)->skill[s.first] <= s.second)
                    ++(*a)->skill[s.first];
                ++a;
            }
        }
    }
}

void Solution2()
{
    ll curtime = 0; list<Project*> cur, next;
    for (Project &p : project)
        next.emplace_back(&p);
    while (true)
    {
        cur.clear();
        cur.swap(next);
        bool added = false;
        for (auto p : cur)
        {
            if (curtime + p->days >= p->roles + p->score)
                continue;
            Result r;
            if (projectTry(*p, r))
            {
                curtime += p->days;
                r.name = p->name;
                sol.emplace_back(r);
                auto a = r.a.begin();
                for (auto s : p->skill)
                {
                    if ((*a)->skill[s.first] <= s.second)
                        ++(*a)->skill[s.first];
                    ++a;
                }
                added = true;
            }
            else
                next.emplace_back(p);
        }
        if (!added)
            break;
    }
}

void Result()
{
    cout << sol.size() << "\n";
    for (auto r : sol)
    {
        cout << r.name << "\n";
        for (auto a : r.a)
            cout << a->name << " ";
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
    for (Counter &c : counter)
        counterRead(c);
    project.resize(pp);
    for (Project &p : project)
        projectRead(p);
    sort(project.begin(), project.end(), compareSkill);

    Solution2();
    Result();
}
