#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cstdlib>

//structure that stores the data for the whole knapsack portion
struct knapsackWhole{
  int Value, Weight;
};

//structure that stores the data for the fractional knapsack portion
struct knapsackFraction{
  int Value, Weight;
  double ValPerWeight;
};

//Dynamic programming method for knapsack
int wholeKnapsack(int W, struct knapsackWhole Whole[], int n, int& TakenWeight)
{
  //Double array which will store the optimal values as it goes through the whole array
  int Sack[n+1][W+1];

  //Zero out first row
  for(int i = 0; i <= W; i++){
    Sack[0][i] = 0;
  }

  //Go through and dynamically create the Sack array
  for(int i = 1; i <= n; i++)
  {
    for(int w = 0; w <= W; w++)
    {
      //If the weight of the previous element is less than the currently selected weight
      if (Whole[i-1].Weight <= w){
      //Take the max value between selecting this item or leaving it out
        Sack[i][w] = std::max(Whole[i-1].Value + Sack[i-1][w-Whole[i-1].Weight], Sack[i-1][w]);
      }
      //If the weight of the previous element exceeds the currently selected weight
      else {
        //Default the current value to the previous value and move on
        Sack[i][w] = Sack[i-1][w];
      }
    }
  }

  for(int i = 0; i <= n; i++){
    for(int j = 0; j <= W; j++){
	std::cout << Sack[i][j] << " ";
    }
    std::cout << std::endl;
  }
  //store the maximum value in a temp variable to calculate the taken weight
  int tempval = Sack[n][W];
  //store the maximum weight given in a temp variable to calculate the taken weight
  int tempweight = W;
  //Set TakenWeight to 0
  TakenWeight = 0;
  //Go through all items and calculate taken weights in reverse
  for(int i = n; i > 0 && tempval > 0; i--){
    //Find the value before that comes before the current highest value
    if(tempval != Sack[i-1][tempweight]){ 
      //Take the weight of that item and add it to the taken weight
      TakenWeight += Whole[i-1].Weight;
      //Subtract the value to find the next change in value
      tempval -= Whole[i - 1].Value; 
      //Subtract the weight to find the value of the next change in value
      tempweight -= Whole[i - 1].Weight; 
    } 
  }
  //return the maximum value at the very end
  return Sack[n][W];
}

//Function that std::sort uses to sort my fractional knapsack structure
bool cmpSort(struct knapsackFraction first, struct knapsackFraction second)
{
  return first.ValPerWeight > second.ValPerWeight;
}

//Get the highest value from the fractional part of the knapsack items
double fractionalKnapsack(int max, struct knapsackFraction Fraction[], int n)
{
  //use cstdlib sort with my compare function to sort Fractional Knapsack in descending order
  std::sort(Fraction, Fraction + n, cmpSort);

  int total = 0; //Total knapsack weight
  double value = 0.0; //Total value from fractional component

  for(int i = 0; i < n; i++)
  {
    //Take whole item if there is enough weight left
    if(total + Fraction[i].Weight <= max)
    {
      total += Fraction[i].Weight;
      value += Fraction[i].Value;
    }

    //Can't take whole item
    else
    {
      //Get how much weight is left
      int remain = max - total;
      //Take the remaining fraction of the item
      value += Fraction[i].Value * ((double) remain / Fraction[i].Weight);
      //return the value
      return value;
    }
  }

  //return the value if you get through all of the items without going past the max weight
  return value;
}

//Generate the inputs for the program at runtime
void inputgen(){
  //Generate 10 inputs with at least one being fractional
  int whole = (rand() % 9) + 1;
  int fraction = 10 - whole;
  //Initialize values for weights and values
  int randweight = -1;
  int randval = -1;

  //Open the output file
  std::ofstream output;
  output.open("input.txt");

  //Write the number of whole components and fractional components
  output << whole << " " << fraction << "\n";

  
  int i = 0;
  int id = 1;
  //Generate the whole components
  for(i; i < whole; i++) {
    //generate a random weight between 1 and 5
    randweight = (rand() % 5) + 1;
    //generate a random value between 5 and 100
    randval = ((rand() % 20) + 1) * 5;
    //Write the whole component to file
    output << "W " << id << " " << randweight << " " << randval << "\n";
    id++;
  }

  i = 0;
  //Generate the fractional components
  for(i; i < fraction; i++) {
    //generate a random weight between 1 and 5
    randweight = (rand() % 5) + 1;
    //generate a random value between 5 and 100
    randval = ((rand() % 20) + 1) * 5;
    //Write the fractional component to file
    output << "F " << id << " " << randweight << " " << randval << "\n";
    id++;
  }
  //close output file
  output.close();
}

int main()
{
  srand(time(NULL));

  inputgen();

  //input stream to read from generated input file
  std::ifstream input;
  //string to hold the contents of each file line
  std::string contents;

  //open the input file
  input.open("input.txt");
  int whole, fraction;
  //read in how many whole and fractional components there are respectively
  input >> whole;
  input >> fraction;

  //Create the arrays of structures to store the Whole and Fractional components
  struct knapsackWhole Whole[whole];
  struct knapsackFraction Fraction[fraction];
  //String to read in the type
  std::string Type;
  //Int to store the ID
  int ID;
  //Index for the Whole section
  int WholeIndex = 0;
  //Index for the Fractional section
  int FractionIndex = 0;
  //Store the lowest recorded weight and the total weight for weight generation
  int LowestWeight = 0x7FFFFFFF;
  int TotalWeight = 0;

  //read from the file
  std::cout << "Here is a list of the generated items:";
  while(getline(input, contents)){
    //split the string by space
    std::istringstream ss(contents);
    //read in the type and id
    ss >> Type;
    ss >> ID;
    //if the current component is whole, store the whole data
    if(Type == "W"){
      std::cout << "Type: " << Type << " ID: " << ID << " ";
      ss >> Whole[WholeIndex].Weight;
      ss >> Whole[WholeIndex].Value;
      std::cout << "Weight: " << Whole[WholeIndex].Weight << " Value: " << Whole[WholeIndex].Value;
      if(Whole[WholeIndex].Weight < LowestWeight){
        LowestWeight = Whole[WholeIndex].Weight;
      }
      TotalWeight += Whole[WholeIndex].Weight;
      WholeIndex++;
    }
    //if the current component if fractional, store the fractional data
    else if(Type == "F"){
      std::cout << "Type: " << Type << " ID: " << ID << " ";
      ss >> Fraction[FractionIndex].Weight;
      ss >> Fraction[FractionIndex].Value;
      Fraction[FractionIndex].ValPerWeight = (double)Fraction[FractionIndex].Value/Fraction[FractionIndex].Weight;
      std::cout << "Weight: " << Fraction[FractionIndex].Weight << " Value: " << Fraction[FractionIndex].Value;
      if(Fraction[FractionIndex].Weight < LowestWeight){
        LowestWeight = Fraction[FractionIndex].Weight;
      }
      TotalWeight += Fraction[FractionIndex].Weight;
      FractionIndex++;
    }
    std::cout << std::endl;
  }

  //Close the input file
  input.close();

  int InputWeight = -1;
  while(InputWeight < LowestWeight || InputWeight > TotalWeight){
	std::cout << "The current total weight of all items is " << TotalWeight << " and the lowest weight is " << LowestWeight << std::endl;
  	std::cout << "Please enter a weight between these values inclusively: ";
	std::cin >> InputWeight;
  }

  //Randomly compute how much of the input weight will be whole
  int WholeWeight = InputWeight - (rand() % InputWeight);

  std::cout << "Attempting to take " << WholeWeight << " pounds from whole knapsack out of total of " << InputWeight << " pounds.\n";

  int TakenWeight = 0;
  //Calculate the whole portion and how much weight was actually used
  int WholePortion = wholeKnapsack(WholeWeight, Whole, WholeIndex, TakenWeight);
  //Generate the fractional portion from the taken weight of the whole portion
  int FractionWeight = InputWeight - TakenWeight;
  std::cout << "Took " << TakenWeight << " pounds from whole knapsack. Fractional portion will be " << FractionWeight << " pounds.\n";
  double FractionalPortion = fractionalKnapsack(FractionWeight, Fraction, FractionIndex);

  std::cout << "The whole value is " << WholePortion << " and the fractional value is " << FractionalPortion << ".\n";
  std::cout << "The total value is " << WholePortion + FractionalPortion << std::endl;

  return 0;
}
