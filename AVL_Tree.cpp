#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

//global variables used
int counter1 = 0; //for counting number of data entries for verify function
int counter2 = 0; //for counting number of balanced nodes for verify function

int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

struct person
{
	int mon, day, year;
	string ssn, fname, lname;
	double bankbal;
	person(string s, int m, int d, int y, string f, string l, double b)
	{
		ssn = s;
		mon = m;
		day = d;
		year = y;
		fname = f;
		lname = l;
		bankbal = b;
	}
	void print()
	{
		cout << fname << " " << lname << " " << day << " " << mon << " " << year << " "; 
		cout << ssn.substr(0,3) << "-" << ssn.substr(3,2) << "-" << ssn.substr(5) << " $" << bankbal << endl;
	}
	void s_print()
	{
		cout << "\n\x1B[35m";
		cout << "First Name: \x1B[33m" << fname;
		cout << "\n\x1B[35m";
		cout << "Last Name: \x1B[33m" << lname;
		cout << "\n\x1B[35m";
		cout << "Birthday: \x1B[33m" << day << " " << mon << " " << year;
		cout << "\n\x1B[35m";
		cout << "Social Security Number: \x1B[33m" << ssn.substr(0,3) << "-" << ssn.substr(3,2) << "-" << ssn.substr(5);
		if(bankbal >= 0)
		{
			cout << "\n\x1B[35m";
			cout << "Amount of Money Saved: \x1B[32m\x1B[1m$" << std::fixed << std::setprecision(2) << bankbal << endl;
		}
		else
			cout << "\n\x1B[31mThe amount of money this person has saved is not in the system!\n";
	}
};

struct node
{
	person * data;
	node * left, * right;
	int height;
	node(person * d)
	{
		data = d;
		left = NULL;
		right = NULL;
		height = 1;
	}
};

void stringModify(string & s)
{
	if(s.length() == 0)
		return;
	if(s[0] >= 'a')	//first letter needs to be capitilized
		s[0] -= 32;
	for(int i = 1; i < s.length(); i++) //rest needs to be lowercase
		if(s[i] < 'a')
			s[i] += 32;
}

int get_height(node * t)
{
	if (t == NULL)
		return 0;
	return t->height;
}

int get_balance(node * t)
{
	if (t == NULL)
		return NULL;
	return get_height(t->left) - get_height(t->right);
}

void printtree(node * tree)
{
	if(tree == NULL)
		return;
	printtree(tree->left);
	cout << "height: " << tree->height << " | bal: " << get_balance(tree) << " | ";
	tree->data->print();
	printtree(tree->right);
}

void insert(person * p, node * & t)
{
	if (t == NULL)
	{
		t = new node(p);
		return;
	}
	if (p->lname < t->data->lname)
		insert(p, t->left);
	else if (p->lname == t->data->lname)
		if(p->fname < t->data->fname)
			insert(p, t->left);
		else
			insert(p, t->right);
	else
		insert(p, t->right);
	t->height = 1 + max(get_height(t->left), get_height(t->right));
	int balance = get_height(t->left) - get_height(t->right);
	//cout << "Balance at this node: " << balance << endl;
	if (balance == 2)
	{
		//fix for balance
		//we know that it was inserted into the left node since the balance is positive 2, so we can pass in the left node to see which case it is
		int yeet = get_balance(t->left);
		if (yeet == 1)
		{
			//printtree(t);
			node * temp = t->left;
			t->left = temp->right;
			temp->right = t;
			t = temp;
			t->right->height = 1 + max(get_height(t->right->left), get_height(t->right->right));
			//cout << t->height << " edited tree +2(+1)\n";
			//printtree(t);
		}
		else if (yeet == -1)
		{
			//printtree(t);
			node * temp = t->left->right;
			t->left->right = temp->left;
			temp->left = t->left;
			node * temp2 = t;
			temp2->left = temp->right;
			temp->right = temp2;
			t = temp;						
			t->height += 1;
			t->right->height -= 2;
			t->left->height -= 1;
			//cout << t->height << " edited tree +2(-1)\n";
			//printtree(t);
		}
	}
	else if (balance == -2)
	{
		//flip side of above fix
		int yeet = get_balance(t->right);
		if (yeet == -1)
		{
			//printtree(t);
			node * temp = t->right;
			t->right = temp->left;
			temp->left = t;
			t = temp;
			t->left->height = 1 + max(get_height(t->right->left), get_height(t->right->right));
			//cout << t->height << " edited tree -2(-1)\n";
			//printtree(t);
		}
		else if (yeet == 1)
		{
			//printtree(t);
			node * temp = t->right->left;
			t->right->left = temp->right;
			temp->right = t->right;
			node * temp2 = t;
			temp2->right = temp->left;
			temp->left = temp2;
			t = temp;						
			t->height += 1;
			t->left->height -= 2;
			t->right->height -= 1;
			//cout << t->height << " edited tree -2(+1)\n";
			//printtree(t);
		}
	}
}

void remove(node * & t, string fname, string lname, bool & found)
{
	while(t != NULL)
	{
		if(lname == t->data->lname && fname == t->data->fname)
		{
			found = true;
			counter1 -= 1;
			break;
		}
		else if (lname < t->data->lname)
			t = t->left;
		else if (lname == t->data->lname)
		{
			if(fname < t->data->fname)
				t = t->left;
			else if(fname > t->data->fname)
				t = t->right;
		}
		else
			t = t->right;
	}
	if(!found)
		return;
	if(t->right == NULL && t->left == NULL)
		t = NULL;
	else if(t->right == NULL && t->left != NULL)
		t = t->left;
	else if(t->right != NULL && t->left == NULL)
		t = t->right;
	else
	{
				int balance = get_height(t->left) - get_height(t->right);
					if(balance == -1)
						;
					else if(balance == 1)
						t = t->left;
					else
						t = t->right;
				}
	t->height = 1 + max(get_height(t->left), get_height(t->right));
	int balance = get_height(t->left) - get_height(t->right);
	//printtree(t);
	cout << "Balance at this node: " << balance << endl;
	if (balance == 2)
	{
		//fix for balance
		//we know that it was inserted into the left node since the balance is positive 2, so we can pass in the left node to see which case it is
		int yeet = get_balance(t->left);
		if (yeet == 1)
		{
			//printtree(t);
			node * temp = t->left;
			t->left = temp->right;
			temp->right = t;
			t = temp;
			t->right->height = 1 + max(get_height(t->right->left), get_height(t->right->right));
			//cout << t->height << " edited tree +2(+1)\n";
			//printtree(t);
		}
		else if (yeet == -1)
		{
			//printtree(t);
			node * temp = t->left->right;
			t->left->right = temp->left;
			temp->left = t->left;
			node * temp2 = t;
			temp2->left = temp->right;
			temp->right = temp2;
			t = temp;						
			t->height += 1;
			t->right->height -= 2;
			t->left->height -= 1;
			//cout << t->height << " edited tree +2(-1)\n";
			//printtree(t);
		}
	}
	else if (balance == -2)
	{
		//flip side of above fix
		int yeet = get_balance(t->right);
		if (yeet == -1)
		{
			//printtree(t);
			node * temp = t->right;
			t->right = temp->left;
			temp->left = t;
			t = temp;
			t->left->height = 1 + max(get_height(t->right->left), get_height(t->right->right));
			//cout << t->height << " edited tree -2(-1)\n";
			//printtree(t);
		}
		else if (yeet == 1)
		{
			//printtree(t);
			node * temp = t->right->left;
			t->right->left = temp->right;
			temp->right = t->right;
			node * temp2 = t;
			temp2->right = temp->left;
			temp->left = temp2;
			t = temp;						
			t->height += 1;
			t->left->height -= 2;
			t->right->height -= 1;
			//cout << t->height << " edited tree -2(+1)\n";
			//printtree(t);
		}
	}
}

void maketree(node * & tree)
{
    ifstream fin("database.txt");
    if(fin.fail())
    {
        cout << "Error opening file!\n";
        exit(1);
    }
    while(true)
    {
        int dy, mh, yr;
        string soc, fna, lna;
		double bb;
        fin >> soc >> dy >> mh >> yr >> fna >> lna >> bb;
        if(fin.fail())
            break;
		person * temp = new person(soc, mh, dy, yr, fna, lna, bb);
		insert(temp, tree);
		counter1++;
		//printtree(tree);
	}
    fin.close();
	cout << "\n\x1B[32m AVL Tree has built successfully!\n";
}

void verify_worker(node * tree)
{
	if(tree->left != NULL)
		verify_worker(tree->left);
	int balance = get_balance(tree);	
	if(balance > -2 && balance < 2)
		counter2++;
	else
		cout << "balance at node was : " << balance << endl;
	if(tree->right != NULL)
		verify_worker(tree->right);
}

void verify(node * tree)
{
	counter2 = 0;
	verify_worker(tree);
	cout << "\nNumber of data items in tree: \x1B[34m" << counter1 << endl;
	cout << "\x1B[0mNumber of nodes in the tree whose balance are at -1, 0, +1: \x1B[34m" << counter2 << endl;
	if(counter1 - counter2 == 0)
		cout << "\x1B[1m\x1B[32mThis AVL Tree is balanced with " << tree->height << " levels!\n";	
}

void search(node * tree, string fn, string ln, bool & found)
{
	if(tree == NULL)
		return;
	if(ln < tree->data->lname)
		search(tree->left, fn, ln, found);
	else if(ln > tree->data->lname)
		search(tree->right, fn, ln, found);
	else if(ln == tree->data->lname)
	{
		if(fn == tree->data->fname)
		{
			tree->data->s_print();
			found = true;
		}
		else if(fn < tree->data->fname)
			search(tree->left, fn, ln, found);
		else if(fn > tree->data->fname)
			search(tree->right, fn, ln, found);
	}
}

void cycle(node * AVL)
{
	while(true)
	{
		string input;
		bool found = false;
		cout << "\n\x1B[0m Enter a command from the list below (without brackets) to play with the AVL tree:\n\n";
		cout << "\x1B[1m\x1B[36m [find] \x1B[0m(will print all info known about person given first and last name)\n";
		cout << "\x1B[1m\x1B[36m [insert] \x1B[0m(insert a new person into the tree)\n";
		cout << "\x1B[1m\x1B[36m [remove] \x1B[0m(remove a person from the tree)\n";
		cout << "\x1B[1m\x1B[36m [verify] \x1B[0m(verify the AVL tree is balanced)\n";
		cout << "\x1B[1m\x1B[36m [print] \x1B[0m(prints entire tree with balance and height at each node)\n";
		cout << "\x1B[31m [exit]\n";
		cout << "\x1B[0m > ";
		cin >> input;
		for(int i = 0; i < input.length(); i++)
			if(input[i] < 'a')
				input[i] += 32;
		if(input == "verify")
			verify(AVL);
		else if(input == "exit")
			exit(1);
		else if(input == "find")
		{
			string fn, ln;
			cout << "\x1B[1m\x1B[36m enter first name\n\x1B[0m > ";
			cin >> fn;
			cout << "\x1B[1m\x1B[36m enter last name\n\x1B[0m > ";			 
			cin >> ln;
			stringModify(fn);
			stringModify(ln);
			search(AVL, fn, ln, found);
			if(!found)
				cout << "\n\x1B[31mPERSON NOT FOUND WITH NAME: " << fn << " " << ln << endl;
		}
		else if(input == "print")
			printtree(AVL);
		else if(input == "insert")
	    {
		    int dy, mh, yr;
    	    string soc, fn, ln;
			double bb;
			cout << "\x1B[1m\x1B[36m Enter the person's first name\n\x1B[0m > ";	cin >> fn;			
			cout << "\x1B[1m\x1B[36m Enter the person's last name\n\x1B[0m > ";	cin >> ln;			
			cout << "\x1B[1m\x1B[36m Enter the person's social security number (no dashes please)\n\x1B[0m > ";	cin >> soc;			
			cout << "\x1B[1m\x1B[36m Enter the day they were born on\n\x1B[0m > ";	cin >> dy;			
			cout << "\x1B[1m\x1B[36m Enter the month they were born in (1 being january, 12 being december)\n\x1B[0m > ";	cin >> mh;			
			cout << "\x1B[1m\x1B[36m Enter the year they were born in\n\x1B[0m > ";	cin >> yr;			
			cout << "\x1B[1m\x1B[36m Enter the amount of money this person has in their bank (Enter -1 if balance is unknwon)\n\x1B[0m > ";	cin >> bb;			
			stringModify(fn);
			stringModify(ln);
			person * temp = new person(soc, mh, dy, yr, fn, ln, bb);
			insert(temp, AVL);
			counter1++; //change this so if persom is already there it doesnt add 1
		}
		else if (input == "remove")
		{
			string fn, ln;
			cout << "\x1B[1m\x1B[36m enter first name\n\x1B[0m > ";
			cin >> fn;
			cout << "\x1B[1m\x1B[36m enter last name\n\x1B[0m > ";			 
			cin >> ln;
			stringModify(fn);
			stringModify(ln);
			remove(AVL, fn, ln, found);
			if(!found)
				cout << "\n\x1B[31mPERSON NOT FOUND WITH NAME: " << fn << " " << ln << endl;
		}
		else
			cout << "\nYou entered an invalid entry!\n";
	}
}

int main()
{
	node * AVL = NULL;
	maketree(AVL);
	cycle(AVL);
}
