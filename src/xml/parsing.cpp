
#include "xml_editor/xml.hpp"
#include <stack>
#include <cctype>
using namespace std;

namespace xml_editor::xml 
{
	Tree* parse(const std::string& input)
     {

        if (input.empty())
        {
            return nullptr;   //check if input is empty
        }

        size_t position = 0;    //current position is at the beginning of string
        stack<TreeNode*> nodeStack;  //stack is empty for TreeNode Pointers
        TreeNode* root = nullptr;    //Root in TreeNode pointer initialized to null

     

        //Skip spaces
        auto skipWhitespace = [&]() {
             while (position < input.size() && isspace(input[position])) 
             position++;
            };
        skipWhitespace();


         while (position < input.size())
          {
                size_t start;
                    if (input[position]=='<')  //Tag detected
                    {

                        //Closing tag
                    if(input[position+1]=='/')
                    {
                        position+=2; 
                        size_t start=position; //Start of tag name

                        while(input[position]!='>'){  
                            position++;
                        }

                        nodeStack.pop(); //tagName is complete
                        position++;  
                        
                    }
                        //Opening tag
                    else
                   {
                    position++; 
                    size_t start = position; //Start of tag name
                   }

                    while(position < input.size() && input[position] != '>' && input[position] != '/')   //idk if i should consider self closing tags or not 
                    {  
                    
                    position++;
                    

                    string tagName = input.substr(start, position - start); //Extract tag name
                    
                    //Create new Node
                    TreeNode* newNode = new TreeNode(tagName, ""); 

                   if (!nodeStack.empty())
                    {
                        nodeStack.top()->add_child(newNode); //adding child to parent node
                    } 
                    else
                    {
                        root = newNode; //If stack is empty, this is the root node (lowest level in stack)
                    }

        
                  }
              }
               
                    //Body text
             else
             {
                size_t start = position; //Start of body text
                while(position < input.size() && input[position] != '<')
            
                 position++;
                    
             
                string body = input.substr(start, position - start);

                if (!nodeStack.empty() && !body.empty())
                {

                size_t first_ch = 0;
                    while (first_ch < body.size() && isspace(body[first_ch])) 
                    
                         first_ch++;  //skip spaces until reach first char
                    
                    size_t last_ch = body.size();

                     while (last_ch > first_ch && isspace(body[last_ch - 1]))
                 
                         last_ch--;  //remove spaces until reach last char
                 
                     if (last_ch > first_ch)
                     {
                            string cleaned = body.substr(first_ch, last_ch - first_ch);
                            nodeStack.top()->set_value(cleaned); //current node being parsed & set its body text
                     }
                 }

            }

                    skipWhitespace(); //Skip whitespace after body text
        }
                    return new Tree(root);  //Return the constructed tree
    }
}


        

    



		
	
