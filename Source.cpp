/*** بسم الله الرحمن الرحيم  ***/
#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#define Fast  std::ios_base::sync_with_stdio(0);cin.tie(NULL);	cout.tie(NULL);
using namespace std;
void file() {
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
}
long long numOfRows = 0, numOfColumn = 0;
vector<pair<string, vector<string>>>dataSet;
void takeInput();
void CreatTree(string name, vector<pair<string, vector<string>>> dataSetTemp, string space);
pair<bool, string> CheckEnd(vector<pair<string, vector<string>>> dataSetTemp);
double Info_D(int idx, vector<pair<string, vector<string>>>dataSetTemp);
int InfoGainDataSet(int idx, vector<pair<string, vector<string>>>dataSetTemp);
map<string, int> NumOfItemsFun(int idx, vector<pair<string, vector<string>>>dataSetTemp);
queue<pair<string, vector<pair<string, vector<string>>>>> dataSetOfItemsFun(int idx, vector<pair<string, vector<string>>>dataSetTEMP);
void printDatasetQueue(queue<pair<string, vector<pair<string, vector<string>>>>>dataSetOfItems);
int main()
{
    Fast  file();
    takeInput();
    CreatTree("", dataSet, "");
}

void takeInput()
{
    cin >> numOfRows >> numOfColumn;
    dataSet.resize(numOfRows);
    for (int i = 0; i < numOfRows; i++)
    {
        //name of coulmn
        cin >> dataSet[i].first;
        for (int j = 0; j < numOfColumn; j++)
        {
            string value;
            cin >> value;
            //push values of the dataset
            dataSet[i].second.push_back(value);
        }
        //cout << dataSet[i].first << "\n";
    }
}

void CreatTree(string name, vector<pair<string, vector<string>>> dataSetTemp, string space)
{
    pair<int, string>Check = CheckEnd(dataSetTemp);
    if (Check.first)
    {
        cout << space << "| " << name << " |\n";
        cout << space << "  " << Check.second << "\n\n";
    }
    else
    {
        long long idxAttributeSelect = 0, idxReturn = -1;
        idxAttributeSelect = dataSetTemp.size() - 1;
        idxReturn = InfoGainDataSet(idxAttributeSelect, dataSetTemp);
        queue<pair<string, vector<pair<string, vector<string>>>>> dataSetOfItems;
        dataSetOfItems = dataSetOfItemsFun(idxReturn, dataSetTemp);
        if (!name.empty())
        {
            cout << space << "| " << name << " |\n";
        }
        cout << space << dataSetTemp[idxReturn].first << "?\n" << space << "{\n";
        while (!dataSetOfItems.empty())
        {
            CreatTree(dataSetOfItems.front().first, dataSetOfItems.front().second, space + "\t");
            dataSetOfItems.pop();
        }
        cout << space << "}\n\n";
    }
}

pair<bool, string> CheckEnd(vector<pair<string, vector<string>>> dataSetTemp)
{
    int idx = dataSetTemp.size() - 1;
    int colSiz = dataSetTemp[idx].second.size();
    map<string, int>counter;
    for (int j = 0; j < colSiz; j++)
    {
        counter[dataSetTemp[idx].second[j]]++;
        if (counter.size() > 1)
            return make_pair(0, "");
    }

    return make_pair(1, counter.begin()->first);
}

double Info_D(int idx, vector<pair<string, vector<string>>>dataSetTemp)
{
    map<string, int>Prob;
    int colSiz = dataSetTemp[idx].second.size();
    for (int j = 0; j < colSiz; j++)
    {
        Prob[dataSetTemp[idx].second[j]]++;
    }
    double GainInfo = 0;
    for (auto i : Prob)
    {
        if (i.second == 0)continue;
        GainInfo += -(i.second / (colSiz * 1.0)) * log2(i.second / (colSiz * 1.0));
    }
    return GainInfo;
}


int InfoGainDataSet(int idx, vector<pair<string, vector<string>>>dataSetTemp)
{
    double InfoD = Info_D(idx, dataSetTemp);
    map<string, map<string, int>>Prob;
    map<string, int>NumOfItems;
    vector<pair<double, int>>AllInfoGain;
    int rowSiz = dataSetTemp.size(), colSiz;
    for (int i = 0; i < rowSiz; i++)
    {
        Prob.clear();
        NumOfItems.clear();
        if (i == idx)continue;
        colSiz = dataSetTemp[i].second.size();
        for (int j = 0; j < colSiz; j++)
        {
            //Every Values there are many values in selected Attribute
            Prob[dataSetTemp[i].second[j]][dataSetTemp[idx].second[j]]++;
            //Calc number of diff valuse
            NumOfItems[dataSetTemp[i].second[j]]++;
        }
        double GainInfo = 0;
        for (auto cycle : Prob)
        {
            double I = 0;
            for (auto j : cycle.second)
            {
                if (j.second == 0)continue;
                I += -(j.second / (NumOfItems[cycle.first] * 1.0)) * log2(j.second / (NumOfItems[cycle.first] * 1.0));

            }
            GainInfo += (NumOfItems[cycle.first] / (colSiz * 1.0)) * I;
        }
        GainInfo = InfoD - GainInfo;
        AllInfoGain.push_back({ GainInfo ,i });
    }
    sort(AllInfoGain.begin(), AllInfoGain.end());
    return AllInfoGain[AllInfoGain.size() - 1].second;
}

map<string, int> NumOfItemsFun(int idx, vector<pair<string, vector<string>>>dataSetTemp)
{
    map<string, int>ret;
    int colSiz = dataSetTemp[idx].second.size();
    for (int j = 0; j < colSiz; j++)
    {
        ret[dataSetTemp[idx].second[j]]++;
    }

    return ret;
}

queue<pair<string, vector<pair<string, vector<string>>>>> dataSetOfItemsFun(int idx, vector<pair<string, vector<string>>>dataSetTEMP)
{
    queue<pair<string, vector<pair<string, vector<string>>>>> dataSetOfItemsRet;
    vector<pair<string, vector<string>>>dataSetTemp;
    map<string, int>NumOfItems;
    NumOfItems = NumOfItemsFun(idx, dataSetTEMP);
    for (auto i : NumOfItems)
    {
        dataSetTemp.clear();
        dataSetTemp.resize(dataSetTEMP.size() - 1);
        int item = 0;
        for (int row = 0; row < dataSetTEMP.size(); row++)
        {
            if (row == idx)continue;
            int colSiz = dataSetTEMP[row].second.size();
            dataSetTemp[item].first = dataSetTEMP[row].first;
            for (int col = 0; col < colSiz; col++)
            {
                if (dataSetTEMP[idx].second[col] == i.first)
                {
                    dataSetTemp[item].second.push_back(dataSetTEMP[row].second[col]);
                }
            }
            item++;
        }
        dataSetOfItemsRet.push({ i.first ,dataSetTemp });
    }

    return dataSetOfItemsRet;
}

void printDatasetQueue(queue<pair<string, vector<pair<string, vector<string>>>>> dataSetOfItems)
{
    vector<pair<string, vector<string>>>dataSetTemp;
    while (!dataSetOfItems.empty())
    {
        dataSetTemp = dataSetOfItems.front().second;
        cout << "\t\t| " << dataSetOfItems.front().first << " |\n";
        dataSetOfItems.pop();
        for (auto i : dataSetTemp)
        {
            cout << i.first << "?\n";
            for (auto j : i.second)
            {
                cout << j << " ";
            }
            cout << "\n";
        }
        cout << "------------------------------------------\n";
    }
}
