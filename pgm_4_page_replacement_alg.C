#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <algorithm>
#include <bits/stdc++.h> 

using namespace std;

bool isElementExists(std::vector<int>& frames, int keyElement){

	bool found = false;

	for (int i = 0; i < frames.size(); i++){
		if (frames[i] == keyElement) {
			found = true;
			break;
		}
	}

	return found;
}


int SimulateFIFO(std::vector<int>& pagelist,int frameSize)
{
	int nextIndex = 0;
	bool frameAvailable = true;
	//changing the default value to -1 from 0, as 0 will be a valid page
	std::vector<int> frames(frameSize,-1);
	std::vector<int>::iterator itr;
	int curPageIndex;
	int pageFaultCnt = 0;
	//gets index of the vector;
	for (itr = pagelist.begin() ; itr != pagelist.end(); ++itr){
		int page = *itr;
		//check page already presents in frames
		bool pageExists = isElementExists(frames, page);
		if (!pageExists ){
			if(frameAvailable){
				frames[nextIndex] = page;
			}else if(!frameAvailable){
				pageFaultCnt++;
				int curPageIndex = std::distance(pagelist.begin(), itr);
				//find victim and replace it
				int victimPage = frames.at(nextIndex);
				frames[nextIndex] = page;
				cout << "Page fault at " <<curPageIndex << " :  page " << victimPage << " replaced by " << page << endl;
			}
			
			nextIndex = nextIndex+1;
			//moving frame index to the begining once, it reached the end
			if(!(nextIndex < frameSize)){
			frameAvailable = false;
			nextIndex = 0;
			}		
		}
		
	}

   return pageFaultCnt;
}



int findLRUVictimPage(std::vector<int>& pagelist, std::vector<int>& frames, int currentPageIndex){
//vector which holds index of pages which are presents in page frame6
std::vector<int> framePageIndex(frames.size(),-1);
auto first =  pagelist.cbegin();
auto last =  pagelist.cbegin() + currentPageIndex;
std::vector<int> subVector(first, last);
int framePage;
for(int index = 0; index<frames.size(); index++){
    	framePage = frames.at(index);
	for (int subIndex = subVector.size()-1; subIndex >= 0; subIndex--){
	//getting the frame page index from pagelist, to get the min index page
	 if(framePage == subVector.at(subIndex)){
		framePageIndex[index] = subIndex;
		break;		
		}
	}
}

int minElementIndex = std::min_element(framePageIndex.begin(),framePageIndex.end()) - framePageIndex.begin();
return minElementIndex;
}


int SimulateLRU(std::vector<int>& pagelist, int frameSize)
{
	int nextIndex = 0;
	bool frameAvailable = true;
	//changing the default value to -1 from 0, as 0 will be a valid page
	std::vector<int> frames(frameSize,-1);
		
	std::vector<int>::iterator itr;
	int curPageIndex;
	int pageFaultCnt = 0;
	//gets index of the vector;
	for (itr = pagelist.begin() ; itr != pagelist.end(); ++itr){
		int page = *itr;
		//check page already presents in frames
		bool pageExists = isElementExists(frames, page);
		if (!pageExists ){
			if(frameAvailable){
				frames[nextIndex] = page;
				nextIndex = nextIndex+1;
				//moving frame index to the begining, once it reached the end
				if(!(nextIndex < frameSize)){
				frameAvailable = false;
				nextIndex = 0;
				}
			}else {
				pageFaultCnt++;
				int curPageIndex = std::distance(pagelist.begin(), itr);
				//find victim and replace it
				int victimPageIndex = findLRUVictimPage(pagelist, frames, curPageIndex);
				int victimPage = frames.at(victimPageIndex);
				frames[victimPageIndex] = page;
				cout << "Page fault at " <<curPageIndex << " :  page " << victimPage << " replaced by " << page << endl;
			}	
		
		}		
	}

	
   return pageFaultCnt;
}


int getNotUsedPageIndex(std::vector<int>& framePageIndex){

	int key = -1;
	//initializing wit invalid index
	int indexToReturn = -1;
	for (int i = 0; i < framePageIndex.size(); i++){
		if (framePageIndex[i] == key) {
			indexToReturn = i;
			break;
		}
	}
	return indexToReturn;
}


int findOptimalVictimPage(std::vector<int>& pagelist, std::vector<int>& frames, int currentPageIndex){
	//while reaching last page in pagelist, replace first page in pageframe
	if(currentPageIndex ==  pagelist.size()-1){
	return 0;
	}

	//vector which holds index of pages which are presents in page frame6
	std::vector<int> framePageIndex(frames.size(),-1);
	auto first =  pagelist.cbegin()+ currentPageIndex+1;
	auto last =  pagelist.cend();
	std::vector<int> subVector(first, last);
	int framePage;
	for(int index = 0; index<frames.size(); index++){
	    	framePage = frames.at(index);
		for (int subIndex = 0; subIndex < subVector.size(); subIndex++){
		
		//getting the frame page index from pagelist, to get the min index page
		 if(framePage == subVector.at(subIndex)){
			framePageIndex[index] = subIndex;
			break;		
			}
		}
	}

	int notUsedPageIndex = getNotUsedPageIndex(framePageIndex);
	int maxElementIndex;
	//all the pages are used in future
	if(notUsedPageIndex == -1){
		maxElementIndex = std::max_element(framePageIndex.begin(),framePageIndex.end()) - framePageIndex.begin();
	}else{
		//replace the page which is not used in future
		maxElementIndex = notUsedPageIndex;
	}
return maxElementIndex;
}


int SimulateOptimal(std::vector<int>& pagelist, int frameSize)
{
   int nextIndex = 0;
	bool frameAvailable = true;
	//changing the default value to -1 from 0, as 0 will be a valid page
	std::vector<int> frames(frameSize,-1);		
	std::vector<int>::iterator itr;
	int curPageIndex;
	int pageFaultCnt = 0;
	//gets index of the vector;
	for (itr = pagelist.begin() ; itr != pagelist.end(); ++itr){
		int page = *itr;
		//check page already presents in frames
		bool pageExists = isElementExists(frames, page);
		if (!pageExists ){
			
			if(frameAvailable){
				frames[nextIndex] = page;
				nextIndex = nextIndex+1;
				//moving frame index to the begining, once it reached the end
				if(!(nextIndex < frameSize)){
					frameAvailable = false;
					nextIndex = 0;
				}
			}else {
				pageFaultCnt++;
				int curPageIndex = std::distance(pagelist.begin(), itr);
				//find victim and replace it
				int victimPageIndex = findOptimalVictimPage(pagelist, frames, curPageIndex);
				int victimPage = frames.at(victimPageIndex);
				frames[victimPageIndex] = page;
				cout << "Page fault at " <<curPageIndex << " :  page " << victimPage << " replaced by " << page << endl;
			}	
		
		}		
	}
	
   return pageFaultCnt;
}

int getSecChanceVictimPage(std::vector<int>& frames, int frameStartIndex, std::map<int, int>& refMap ){
int victimPage = -1;
int referenceBit = -1;
	//start looking for victim page after the page, which was replaced last time
	for (int frameIndex = frameStartIndex; frameIndex < frames.size(); frameIndex++){
		//get reference bit from map
		std::map<int, int>::iterator mapItr = refMap.find(frames[frameIndex]); 
		if (mapItr != refMap.end()){
		   	referenceBit = mapItr->second;
			if(referenceBit == 0){
				victimPage = mapItr->first;
				break;
			}else if(referenceBit == 1){
				//giving second chance to the page
				mapItr->second = 0;
			}
		}
		//still not finding the victimpage and loop reached end, continues search from beginning
		if( victimPage == -1 && frameIndex == frames.size()-1){
			//resetting index to -1, so that while doing 'frameIndex++', loop from index 0
			frameIndex = -1;
		}
	}
return victimPage;
}

//gets index of the victim page in frames vector 
int getVictimPageFrameIndex(std::vector<int>& frames, int victimPage){
	std::vector<int>::iterator itr = std::find(frames.begin(), frames.end(), victimPage);
	int victimIndex = std::distance(frames.begin(), itr);
	return victimIndex;
}

int SimulateSecondChance(std::vector<int>& pagelist, int frameSize)
{
	int nextIndex = 0;
	bool frameAvailable = true;
	//changing the default value to -1 from 0, as 0 will be a valid page
	std::vector<int> frames(frameSize,-1);
	int pageFaultCnt = 0;
	//vector to hold reference bit of each page, default value =0
	std::map<int, int> pageReference;

	for (int pageIndex=0; pageIndex < pagelist.size(); pageIndex++){
		int page = pagelist[pageIndex];
		pageReference.insert(std::make_pair(page, 0)); 
		//check page already presents in frames
		bool pageExists = isElementExists(frames, page);
		if (pageExists ){
			//updating refence bit to 1 for the current page
			std::map<int, int>::iterator mapItr = pageReference.find(page); 
			if (mapItr != pageReference.end()){
			    mapItr->second = 1;
			}
		}else{
			if(frameAvailable){
				frames[nextIndex] = page;
				nextIndex = nextIndex + 1;
			}else if(!frameAvailable){
				pageFaultCnt++;
				//find victim and replace it
				int victimPage = getSecChanceVictimPage(frames, nextIndex, pageReference );
				int victimPageIdx = getVictimPageFrameIndex(frames, victimPage);
				frames[victimPageIdx] = page;
				cout << "Page fault at index " <<  pageIndex << " :  page " << victimPage << " replaced by " << page << endl;
				nextIndex = victimPageIdx + 1;
			}			
			
			//moving frame index to the begining once, it reached the end
			if(!(nextIndex < frameSize)){
			frameAvailable = false;
			nextIndex = 0;
			}		
		}
		
	}

   return pageFaultCnt;
}

int resetRefBit(std::map<int, int>& refMap){
	std::map<int, int>::iterator mapItr;
	for (mapItr = refMap.begin(); mapItr != refMap.end(); ++mapItr) { 
              mapItr->second = 0; 
   	}
}

int getLFUVictimPage(std::vector<int>& frames, std::map<int, int>& refMap, int currentPage ){
int victimPage = -1;
	//start looking for victim page from reference map, which page has 0 bit
	auto itr = refMap.begin();
	while(itr != refMap.end())
	{
		if( (itr->first != currentPage) && (itr->second == 0) )
		{
			victimPage = itr->first;
			break;
		}
		itr++;
	}
		
		//still not finding the victimpage and loop reached end, resets reference bit to 0 for all the pages
		//and returns first page as victim page
		if(victimPage == -1){
			resetRefBit(refMap);
			victimPage = frames[0];
		}
return victimPage;
}


int SimulateApproxLFU(vector<int>& pagelist, int frameSize)
{
	int nextIndex = 0;
	bool frameAvailable = true;
	//changing the default value to -1 from 0, as 0 will be a valid page
	std::vector<int> frames(frameSize,-1);
	int pageFaultCnt = 0;
	//vector to hold reference bit of each page, default value =0
	std::map<int, int> pageReference;

	for (int pageIndex=0; pageIndex < pagelist.size(); pageIndex++){
		int page = pagelist[pageIndex];
		pageReference.insert(std::make_pair(page, 0)); 
		//check page already presents in frames
		bool pageExists = isElementExists(frames, page);
		if (pageExists ){
			//updating refence bit to 1 for the current page
			std::map<int, int>::iterator mapItr = pageReference.find(page); 
			if (mapItr != pageReference.end()){
			    mapItr->second = 1;
			}
		}else{
			if(frameAvailable){
				frames[nextIndex] = page;
				nextIndex = nextIndex + 1;
			}else if(!frameAvailable){
				pageFaultCnt++;
				//find victim and replace it
				//int victimPage = getLFUVictimPage(frames, nextIndex, pageReference );
				int victimPage = getLFUVictimPage(frames, pageReference,page );
				int victimPageIdx = getVictimPageFrameIndex(frames, victimPage);
				frames[victimPageIdx] = page;
				cout << "Page fault at index " <<  pageIndex << " :  page " << victimPage << " replaced by " << page << endl;
				//removing the victim page from reference map
				pageReference.erase (victimPage);   
				nextIndex = victimPageIdx + 1;
			}			
			
			//moving frame index to the begining once, it reached the end
			if(!(nextIndex < frameSize)){
			frameAvailable = false;
			nextIndex = 0;
			}		
		}		
	}

   return pageFaultCnt;
}
