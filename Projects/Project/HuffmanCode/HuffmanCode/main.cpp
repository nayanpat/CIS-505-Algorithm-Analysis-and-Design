/******* AUTHOR: Nayan Patel *****************
******* COURSE: CIS 505-002 ******************
******* TERM: Fall 2021 **********************
******* Faculty: Prof. Thomas Steiner ********
******* Project: Item 1 **********************
******* Program Name: C++ Code for ***********
******* Huffman Encoding Algo ***************/

// Including header files
#include <bits/stdc++.h>
#include <chrono>

using namespace std; 
using namespace chrono;

#define CHAR_BIT_LENGTH 8
#define WHITE_SPACE     32

map<char, string> encodedBits;

/* Structure for Huffman tree node */
typedef struct node 
{
    // symbol
    char sym;

    // Frequency of the symbol
    float freq;

    // Left and right child node

    node *left, *right;

    node(char sym, float freq)
    {
        left = right = NULL;
        this->sym = sym;
        this->freq = freq; 
    }
} node;

/* Comparing two heap nodes */
struct compare 
{
    bool operator()(node* l, node* r)
	{
		return (l->freq > r->freq); 
	}
};

/* Get the code for each symbol from the priority queue */  
void getCodes(struct node* root, string str) 
{ 
    if (!root) 
        return; 

    if (root->sym != '$') 
	{
		encodedBits.insert(pair <char, string> (root->sym, str));
	}
 
    getCodes(root->left, str + "0"); 
    getCodes(root->right, str + "1"); 
} 

/* Function that builds a Huffman Tree */
void buildHuffmanTree(char sym[], float freq[], int size) 
{ 
    struct node *left, *right, *top; 

    // Create a priority queue & inserts all characters of sym[] 
    priority_queue <node*, vector<node*>, compare> PQ; 

    for (int i = 0; i < size; ++i) 
	{
        PQ.push(new node(sym[i], freq[i])); 		
	}

    // Iterate while size of priority queue doesn't become 1 
    while (PQ.size() != 1) 
	{ 

        // Extract the two minimum 
        // frequency items from priority queue 
        left = PQ.top(); 
        PQ.pop(); 

        right = PQ.top(); 
        PQ.pop(); 

        // Create a new internal node with 
        // frequency equal to the sum of the 
        // two nodes frequencies. Make the 
        // two extracted node as left and right children 
        // of this new node. Add this node 
        // to the priority queue '$' is a special value 
        // for internal nodes, not used 
        top = new node('$', left->freq + right->freq); 

        top->left = left; 
        top->right = right; 

        PQ.push(top); 
    } 

    // Get Huffman codes using 
    // the Huffman tree built above 
	getCodes(PQ.top(), "");
} 

/* Function to sort the symbols based on their probability or frequency 
 * NOTE:  This is purely for printing the results and NOT USED in the 
 * actual huffman algorithm */
void sortByFrequency(char sym[], float freq[], int n)
{
	char tempData = ' ';
	float tempFreq = 0.0;
	for (int j = 0; j < n; j++) 
	{
		for (int i = 0; i < n - 1; i++) 
		{
			if (freq[i] < freq[i + 1]) 
			{
				tempFreq = freq[i];
				tempData = sym[i];

				freq[i] = freq[i + 1];
				sym[i] = sym[i + 1];

				freq[i + 1] = tempFreq;
				sym[i + 1] = tempData;
			}
		}
	}
}

/* Main function to drive the huffman code Contains two arguments: (1) the executable,
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
		
		char sym[n] = {' '}; 
		int occurance[n] = {0}; 
		float freq[n] = {0}; 
		
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

		// To work with buildHuffmanTree(), store the map data to separate arrays
		int idx = 0;
		for (auto& x: histogram)  
		{
			// Insert the symbol to array sym[]
			sym[idx] = x.first;
			
			// Insert the value to array fre[]
			occurance[idx] = x.second;
			freq[idx] = x.second / total_occ;
			idx++;
		}
		
		// Calculating entropy of symbols based on their frequency
		for(int i = 0; i < n; i++)
		{
			entropy += (-( freq[i] * log2 (freq[i]) ) );	
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
		
		// Call buildHuffmanTree() function and pass symbol, frequency and number of samples
		buildHuffmanTree(sym, freq, n); 

		// Get number of encoded character bits for each symbol
		int encodedCharBits[n] = {0};
		for(int i = 0; i < n; i++)
		{
			encodedCharBits[i] = encodedBits[sym[i]].length();
		}

		// Calculate total bits required to store each character without
		// and with encoding
		for(int i = 0; i < n; i++)
		{
			originalTotalBits += occurance[i] * CHAR_BIT_LENGTH;
			encodedTotalBits += occurance[i] * encodedCharBits[i];	
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
		sortByFrequency(sym, freq, n);
		cout << "Symbol   " << "Frequency\t" << "code\t\t" << endl;
		for(int i = 0; i < n; i++)
		{
			cout << sym[i] << "\t" << freq[i] << "\t" << encodedBits[sym[i]] << endl;
		}	

		cout << endl;
		cout << "Total original bit length is " << originalTotalBits << endl;
		cout << "Total encoded bit length is " << encodedTotalBits << endl;
		cout << "Entropy is " << entropy << endl;
		cout << "Binary code length based on min fixed length coding = " << fixedLengthCode << endl;		
		cout << "Average Bit length after encoding is " << avgBits << endl;
		cout << "Efficiency for binary code is " << efficBeforeCompress << endl;
		cout << "Efficiency of huffman coding is " << efficAfterCompress << endl;
	
		cout << "Compression ratio for file " << fileName << " is " << compression << endl;

		// Stop the clock and calculate the delta to measure the actual execution time for huffman coding 
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start).count();

		cout << "Total Execution Time for Huffman encoding algo (in microseconds) for file " 
			<< fileName << " is " << duration << setprecision(9) << endl;
	}
	// If number of arguments are incorrect, print error message
	else
	{
		cout << "Wrong number of arguments, please check and run program with right arguments" << endl;
	}	
	
    return 0; 
}