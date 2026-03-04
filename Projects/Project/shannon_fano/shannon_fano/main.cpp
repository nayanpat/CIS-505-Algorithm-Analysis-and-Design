/****** AUTHOR: Nayan Patel ******************
******* COURSE: CIS 505-002 ******************
******* TERM: Fall 2021 **********************
******* Faculty: Prof. Thomas Steiner ********
******* Project: Item 1 **********************
******* Program Name: C++ Code for ***********
******* Shannon-Fano Encoding Algo **********/

// include header files
#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace chrono;

#define HALF 			2.0f
#define CHAR_BIT_LENGTH 8
#define WHITE_SPACE     32

/* Structure for Shannon-Fano tree node */
typedef struct node 
{

	// for storing symbol
	string sym;

	// for storing frequency
	int occ;
	float freq;
	int arr[16];
	int top;
}node;

// An instance of the node structure to store symbolds, frequency, code and length of code
struct node p[128];

/* Function that builds a shannon-fano Tree */
void buildShannonFanoTree(int l, int h, node p[])
{
	int i = 0, k = 0;
	float total = 0, halfTotal = 0, leftTotal = 0;
	\
	// If no more samples left for processing, return
	if (l == h || l > h)
	{
		return;			
	}
	
	// If last two samples left for processing, assignm them the code and return
	if ( (l + 1) == h ) 
	{		
		p[l].arr[++(p[l].top)] = 0;
		p[h].arr[++(p[h].top)] = 1;
		return;
	}
	// Otherwise, divide the samples into two as equal as possible parts
	else 
	{
		for(i = l; i < h; i++)
		{
			total += p[i].freq;
		}
		
		halfTotal = total / HALF;
		
		for(i = l; i < h; i++)
		{
			leftTotal += p[i].freq;
			
			if(leftTotal >= halfTotal)
			{
				k = i;
				break;
			}
		}	
		
		// Assing add 0 to all the left nodes 
		for (i = l; i <= k; i++)
		{
			p[i].arr[++(p[i].top)] = 0;			
		}
		
		// Assing add 1 to all the left nodes 
		for (i = k + 1; i <= h; i++)
		{
			p[i].arr[++(p[i].top)] = 1;			
		}

		// Recursive calls to buildShannonFanoTree function
		buildShannonFanoTree(l, k, p);
		buildShannonFanoTree(k + 1, h, p);
	}	
}

/* Function to sort the symbols based on their frequency */
void sortByFrequency(int n, node p[])
{
	int i, j;
	node temp;
	for (j = 0; j < n; j++) 
	{
		for (i = 0; i < n - 1; i++) 
		{
			if ((p[i].freq) < (p[i + 1].freq)) 
			{
				temp.freq = p[i].freq;
				temp.occ = p[i].occ;
				temp.sym = p[i].sym;

				p[i].freq = p[i + 1].freq;
				p[i].occ = p[i + 1].occ;
				p[i].sym = p[i + 1].sym;

				p[i + 1].freq = temp.freq;
				p[i + 1].occ = temp.occ;
				p[i + 1].sym = temp.sym;
			}
		}
	}
}

/* function to print shannon codes */
void printSFCodes(int n, node p[])
{
	int i, j;
	cout << "\n\tSymbol\t\tFrequency\tCode";
	for (i = 0; i < n; i++) 
	{
		cout << "\n\t" << p[i].sym << "\t\t" << p[i].freq << "\t";
		for (j = 0; j <= p[i].top; j++)
			cout << p[i].arr[j];
	}
}

/* Main function to drive the shannon-fano code Contains two arguments: (1) the executable,
* (2) input file name that needs compression */
int main(int argc, char* argv[]) 
{ 		
	cout << "Number of argeuments is = " << argc << " and input file for compression is "<< argv[1] << endl;	
	string fileName = argv[1];

	/* Input arguments have to be exact 2 */
	if(argc == 2)
	{
		char ch = 0;
		int n = 0; 
		int total_occ = 0;
		int spaceCounts = 0;
		float total = 0;
		long originalTotalBits = 0;
		long encodedTotalBits = 0;
		float entropy = 0.0;
		float fixedLengthCode = 0.0;
		float avgBits = 0.0;
		float efficAfterCompress = 0.0;
		float efficBeforeCompress = 0.0;
		float compression = 0.0;

		ifstream in("C:/Users/NPATE145/Documents/Personal Docs/MS in AI/Courses/CIS 505 Algo Design/Project/Sample Database/" + fileName);

		// Map to store counts of each symbol.
		map<char, float> histogram; 

		// Read each symbol from the input file and store it in the map		
		while (in.get(ch)) 
		{
			if (isspace(ch))
			{
				++spaceCounts;
			}
			else
			{
				++histogram[ch];
			}			
		}
		
		histogram.insert(pair <char, float> (WHITE_SPACE, spaceCounts));
		
		// Get the number of symbols (i.e. samples)
		n = histogram.size();

		cout << "Total number of samples = " << n << endl;
		
		// Calculate total occurance of all the symbols in the file
		for(auto& x: histogram)
		{
			total_occ += x.second;
		} 
		
		// Print symbols and their occurances
		for (auto& x: histogram)
		{
			cout << "Symbol " << x.first << " & " << "Occurance = " << x.second << endl;
		}
		
		// To work with buildShannonFanoTree(), store the map data to node p[]
		int idx = 0;
		for (auto& x: histogram)  
		{
			// Insert the symbol to node
			p[idx].sym = x.first;
			
			// Insert the value to node
			p[idx].occ = x.second;
			p[idx].freq = x.second / total_occ;
			total = total + p[idx].freq;
			idx++;
		}
		
		// Calculating entropy of symbols based on their frequency
		for(int i = 0; i < n; i++)
		{
			entropy += (-( p[i].freq * log2 (p[i].freq) ) );	
		}
		
		// Minimum bit length required to store each symbol based on fixed length coding
		for(int i = 0; i < n; i++)
		{
			if(pow(2, i) >= n)
			{
				fixedLengthCode = i;
				break;
			}
		}
		
		// Start the clock to measure the huffman algorithm timing
		auto start = high_resolution_clock::now();	   
		
		// Sorting the symbols based on their frequency in decreasing order
		sortByFrequency(n, p);

		// Initializing the symbol code length to -1
		for (int i = 0; i < n; i++)
		{
			p[i].top = -1;
		}

		// Call buildShannonFanoTree() function and pass first and end sample index and node p[]
		buildShannonFanoTree(0, n, p);

		// Calculate total bits required to store each character without
		// and with encoding
		for(int i = 0; i < n; i++)
		{
			originalTotalBits += p[i].occ * CHAR_BIT_LENGTH;
			encodedTotalBits += p[i].occ * (p[i].top + 1);	
		}

		// Efficiency before the compression
		efficBeforeCompress = (entropy / fixedLengthCode) * 100.0;
		// Average bit size of encoded symbols
		avgBits = (float)encodedTotalBits / (float)total_occ;
		// Efficiency after the compression
		efficAfterCompress = (entropy / avgBits) * 100.0;
		// % Compression 
		compression = ((float)originalTotalBits) / (float)encodedTotalBits * 100.0;	

		/********** Print all the useful information calculated previously ***********/	
		printSFCodes(n, p);
		cout << endl;
		cout << "Total original bit length is " << originalTotalBits << endl;
		cout << "Total encoded bit length is " << encodedTotalBits << endl;
		cout << "Entropy is " << entropy << endl;
		cout << "Binary code length based on min fixed length coding = " << fixedLengthCode << endl;		
		cout << "Average Bit length after encoding is " << avgBits << endl;
		cout << "Efficiency for binary code is " << efficBeforeCompress << endl;
		cout << "Efficiency of shannon-fano coding is " << efficAfterCompress << endl;
	
		cout << "Compression ratio for file " << fileName << " is " << compression << endl;

		// Stop the clock and calculate the delta to measure the actual execution time for huffman coding 
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start).count();
		
		cout << "Total Execution Time for Shannon-Fano encoding algo (in microseconds) for file " << fileName << " is " << duration << setprecision(9) << endl;
	}
	// If number of arguments are incorrect, print error message
	else
	{
		cout << "Wrong number of arguments, please check and run program with right arguments" << endl;
	}	
	
	return 0;
}
