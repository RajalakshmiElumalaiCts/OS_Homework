//#include <stdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
   These are the page replacement algorithms to be implemented

   For each algorithm:

   -- return the number of page faults (including loading the pages from the beginning)
   -- whenever there is a page fault, print the following
      -- when does the page fault occurs (in terms of location of the pagelist vector
      -- which page is being replaced
      -- which page is being brought in
      -- for printing, use the following commands (you can use your own variable name)
         cout << "Page fault at " << page_fault_location << " :  page " << replaced_page << " replaced by " << page_brought_in << endl;


   -- for all algorithms (except Optimal), whenever there is a page fault, you CANNOT look at pagelist to determine which page to replace
*/

/*
int SimulateFIFO(vector<int>& pagelist, int f)
{
   return 0;
}

int SimulateOptimal(vector<int>& pagelist, int f)
{
   return 0;
}

int SimulateLRU(vector<int>& pagelist, int f)
{
   return 0;
}

int SimulateSecondChance(vector<int>& pagelist, int f)
{
   return 0;
}

int SimulateApproxLFU(vector<int>& pagelist, int f)
{
   return 0;
}
*/

int SimulateFIFO(vector<int>& pagelist, int f);
int SimulateOptimal(vector<int>& pagelist, int f);
int SimulateLRU(vector<int>& pagelist, int f);
int SimulateSecondChance(vector<int>& pagelist, int f);
int SimulateApproxLFU(vector<int>& pagelist, int f);

void GenPageList(vector<int>& pagelist, int length, int workingSetSize) 
{
   int curstart = 0;
   for (int i = 0; i < length; i++)
	{
	   if (rand() % 100 > 90)
	     pagelist.push_back(curstart + rand() % 500);
	   else
	     pagelist.push_back(curstart + rand() % workingSetSize);
	   if (rand() % 100 > 90)
              curstart = rand() % 500;
	   else if (rand() % 100 > 75)
              curstart = (curstart + rand() % workingSetSize + workingSetSize / 4)  % 500;
	}
}


main()
{
   vector<int> pageorder;
   int minframeavail, maxframeavail, listLength, wSize; 
   vector<int> pagelist;

   srand(time(0));
   cin >> listLength >> wSize;
   cin >> minframeavail >>  maxframeavail;
   GenPageList(pagelist, listLength, wSize);
   cout << "Page list : " << endl;
   for (int i = 0; i < listLength; i++)
	cout << pagelist[i] << " ";
   cout << endl;

   for (int f = minframeavail; f <= maxframeavail; f++)
   {
      cout << "Frames available : " << f << endl;
      cout << "    FIFO    : " << SimulateFIFO(pagelist, f) << endl;
      cout << "    Opti    : " << SimulateOptimal(pagelist, f) << endl;
      cout << "    LRU     : " << SimulateLRU(pagelist, f) << endl;
      cout << "    2nd     : " << SimulateSecondChance(pagelist, f) << endl;
      cout << "    aLFU    : " << SimulateApproxLFU(pagelist, f) << endl;
      cout << endl;
   }

}
