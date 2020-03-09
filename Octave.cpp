#include <iostream>
#include <stdlib.h>
#include <iterator> 
#include <list> 
#include <string>
#include <cctype>
using namespace std;

struct token{
    string type;
    string value;
};

struct symbol{
    struct token tk;
    struct symbol* leftsym;
    struct symbol* rightsym;
    
};

struct dem{
    int x;
    int y;
    
};

struct symbol* root=NULL;
int lexer(string line);
struct symbol* newNode(string tk,string tkt,struct symbol* left,struct symbol* right);
void parser();
string getnextToken(list<list<string> >::iterator itr);
string getnextTokenValue(list<list<string> >::iterator itr);
double evaluateTree(struct symbol* node);
struct symbol* factor();
struct symbol* term();
struct symbol* expression();
void consume(string token);
string peek();
list<list<string> >::iterator itr;
string listfactor();
struct symbol* listexp();
struct symbol* listmult();
struct symbol* listaddsub();
string evaluateVectorTree(struct symbol* node);
string VectorNumber(string vector,string num,string op);
string VectorVector(string vector1,string vector2,string op);
string MatrixNumber(string matrix,string num,string op);
string MatrixMatrix(string matrix1,string matrix2,string op);
string MatrixVector(string matrix,string vector,string op);
string MatrixMult(string matrix1,int x,int y,string matrix2);
string findtype(string str);
struct dem findDem(string vector);
int sorvorm=0;
int numflag=0,vecflag=0;

int main(){
    string line;
    cout<<"Welcome to Universal Complex Scripting Calculator by UCSC, Sri Lanka.\nVersion:  1.0\n";
    while (cout<<">> " && getline(cin, line) && line!="exit"){
        if(line.length()==0)
            continue;
        else{
            int i=0,flag=0;
            while(i<line.length()){
                if(!isspace(line[i])){
                    flag=1;
                    break;
                }
                i++;
            }
            if(flag==0)
                continue;
        }
        lexer(line);
    }
    return 0;
}


struct symbol* newNode(string tk,string tkt,struct symbol* left,struct symbol* right)
{ 
  struct symbol* node = new symbol{}; 
  node->tk.type=tkt;
  node->tk.value=tk;  
  node->leftsym = left; 
  node->rightsym = right; 
  return node; 
} 
 
int lexer(string line){
    string str=line;
    list<list<string> > arr;
    int i=0;
    int flag=0;
    while(i<str.length()){
        
        if(isspace(str[i])&&flag==0){
            i++;
        }
        else if(isspace(str[i]) && flag==1){
            list<string> single_list;

            list<string> back=arr.back();
            list<std::string>::iterator it = back.begin();
            advance(it,1);
            if(*it!=";" && *it!="["){
                single_list.push_back("space");
                single_list.push_back(" ");  
                arr.push_back(single_list);
            }
            i++;
            while(isspace(str[i])){
                i++;
            } 
        }
        else if(str[i]=='+'||str[i]=='-'||str[i]=='*'||str[i]=='('||str[i]==')'||str[i]=='['||str[i]==']'||str[i]==','||str[i]==';'||(i+1<str.length() &&(str[i]=='.' && str[i+1]=='*'))){
            list<string> single_list;            
            
            if(str[i]=='+'){
               single_list.push_back("operator");
               single_list.push_back("+");
               i++;
            }
            else if(str[i]=='-'){
               single_list.push_back("operator");
               single_list.push_back("-");
               i++;
            }
            else if(str[i]=='*'){
               single_list.push_back("operator");
               single_list.push_back("*"); 
               i++;
            } 
             else if(str[i]=='('){
                 single_list.push_back("lparenthisis");
               single_list.push_back("("); 
               i++;
            } 
             else if(str[i]==')'){
               single_list.push_back("rparenthisis");
               single_list.push_back(")"); 
               i++;
            } 
             else if(str[i]=='['){
               flag=1;
               single_list.push_back("lparenthisis");
               single_list.push_back("["); 
               i++;
            } 
             else if(str[i]==']'){
               flag=0;
                list<string> back=arr.back();
                list<std::string>::iterator it = back.begin();
                if(*it=="space"){
                    arr.pop_back();
                }
               single_list.push_back("rparenthisis");
               single_list.push_back("]"); 
               i++;
            } 
            else if(str[i]==','){
               single_list.push_back("operator");
               single_list.push_back(","); 
               i++;
            } 
            else if(str[i]==';'){
                list<string> back=arr.back();
                list<std::string>::iterator it = back.begin();
                if(*it=="space"){
                   arr.pop_back();
                }
               single_list.push_back("operator");
               single_list.push_back(";"); 
               i++;
            } 
            else if(str[i]=='.'&& str[i+1]=='*'){
               single_list.push_back("operator");
               single_list.push_back(".*"); 
               i=i+2;
            }
            arr.push_back(single_list);
            
        }
        else if(isdigit(str[i])){
            list<string> single_list;
            single_list.push_back("number");
            string num="";
            while(isdigit(str[i])){
                num+=str[i];
                i++;
            }
            
            if(str[i]=='.'){  
                num+=".";              
                i++;
                while(isdigit(str[i])){
                   num+=str[i];
                   i++; 
                }
            }
            single_list.push_back(num); 
            arr.push_back(single_list);            
            
        }
        else{
            cout<<"Token not recognized\n";
            return 0;
        }        
    }
    list<string> single_list;
    single_list.push_back("end");
    single_list.push_back("end");
    arr.push_back(single_list);
    itr=arr.begin();    
    parser(); 
    return 0;
}

void parser(){    
    root=expression();   
    if(sorvorm==0 && numflag==0){
        cout<<"ans = "<<evaluateTree(root)<<"\n";        
    }
    else if(sorvorm==1 && vecflag==0){             
        string str=evaluateVectorTree(root);
        if(str!=""){
            cout<<"ans =\n   ";
            int i=0;
            while(i<str.length()){
                if(str[i]=='['||str[i]==']'){
                    i++;
                    continue;
                }
                else if(str[i]==','){
                    cout<<"   ";
                    i++;
                }
                else if(str[i]==';'){
                    cout<<"\n   ";
                    i++;
                }
                else{
                    cout<<str[i];
                    i++;
                }
            }
            cout<<"\n";
            sorvorm=0;
        }
    }
    numflag=0;
    vecflag=0;    
}

string peek(){        
        list<list<string> >::iterator nested_list_itr=itr;         
        list<string>::iterator single_list_itr;
        list<string>& single_list_pointer = *nested_list_itr;  
        single_list_itr=single_list_pointer.begin();      
        return *(++single_list_itr);
}

void consume(string token){
    if(token!=getnextTokenValue(itr)){
        cout<<"Invalid Syntax\n";         
    }
    if(getnextTokenValue(itr)!="end")
        itr++;    
}

string listfactor(){
    string tk=peek();
    if(getnextToken(itr)=="number"){
        consume(tk);   
        return tk;
    } 
    else if(tk=="+"){
        consume(tk);
        tk=peek();
        if(getnextToken(itr)!="number"){
            cout<<"Unexpected Token\n";
            vecflag=1;
           return ""; 
        } 
        consume(tk);
        return tk;
    }
    else if(tk=="-"){
        consume(tk);
        tk=peek();
        if(getnextToken(itr)!="number"){
            cout<<"Unexpected Token\n";
            vecflag=1;
           return ""; 
        }        
        consume(tk);
        return "-"+tk;
    }         
    else {
           cout<<"Unexpected Token\n";
           vecflag=1;
           return ""; 
        }
}

struct symbol* listexp(){
    string rhs=listfactor();
    struct symbol* exp=newNode("["+rhs+"]","vector",NULL,NULL);    
    string tk=peek(); 
    while(tk==","||tk==";"||tk==" "){
        consume(tk);
        if(tk==" ")
            tk=",";
        rhs=rhs+tk+listfactor();
        exp=newNode("["+rhs+"]","vector",NULL,NULL);
        tk=peek();
    }
    rhs="["+rhs+"]";
    int i=0,x=0,y=1,x1=0;
    while (i<rhs.length()) {
        if(rhs[i]=='['){
            i++;
            continue;
        }
        if(rhs[i]==']'){
            if(x1!=0){
                if(x!=x1){
                    cout<<"Demensions Mismatch\n";
                    vecflag=1;
                    return NULL;
                }
            }
            if(x1==0)
                x1=x;
            i++;
            continue;
        }
        else if(rhs[i]==','||rhs[i]=='-'){
                i++;
                continue;
        }
        else if(rhs[i]==';'){
                if(x1!=0){
                    if(x!=x1){
                        cout<<"Demensions Mismatch\n";
                        vecflag=1;
                        return NULL;
                    }
                }
                if(x1==0)
                    x1=x;
                x=0;
                i++;
                y++;
                continue;
        } 
        string mid="";
        while(isdigit(rhs[i])){
            mid+=rhs[i];
            i++;
        }                
        if(rhs[i]=='.'){  
            mid+=".";              
            i++;
            while(isdigit(rhs[i])){
                mid+=rhs[i];
                i++; 
            }
        }
        x++;
    }    
    return exp;
}

struct symbol* listaddsub(){
    struct symbol* exp=listmult();
    string tk=peek();
    while(tk=="+"||tk=="-"){         
        consume(tk);
        struct symbol* rhs=listmult();
        exp=newNode(tk,"operator",exp,rhs);
        tk=peek();
    }
    return exp;
}

struct symbol* listmult(){  
  struct symbol* exp=listexp();
  string tk=peek();  
  while(tk=="*"||tk==".*"){    
      consume(tk);      
      struct symbol* rhs=listexp();
      exp=newNode(tk,"operator",exp,rhs);
      tk=peek();
  }
  return exp;
}

struct symbol* expression(){
    struct symbol* exp=term();
    string tk=peek();
    while(tk=="+"||tk=="-"){         
        consume(tk);
        struct symbol* rhs=term();
        exp=newNode(tk,"operator",exp,rhs);
        tk=peek();
    }
    if(tk!="end" && tk!=")" && (tk!="+"||tk!="-")){
        cout<<"Unexpected Tokens\n";
        numflag=1;
        return NULL;
    }      
    return exp;
}

struct symbol* term(){  
  struct symbol* exp=factor();
  string tk=peek();
  while(tk=="*"||tk==".*"){    
      consume(tk);      
      struct symbol* rhs=factor();
      exp=newNode(tk,"operator",exp,rhs);
      tk=peek();
  }  
  return exp;
}

struct symbol* factor(){
    string tk=peek();
    if(getnextToken(itr)=="number"){       
        struct symbol* t= newNode(tk,"number",NULL,NULL);         
        consume(tk);     
        return t;
    }
    else if(tk=="+"){
        consume(tk);
        tk=peek();
        if(getnextToken(itr)!="number"){
            cout<<"Unexpected Token\n";
            numflag=1;
           return NULL; 
        }        
        struct symbol* t= newNode(tk,"number",NULL,NULL);
        consume(tk);
        return t;
    }
    else if(tk=="-"){
        consume(tk);
        tk=peek();
        if(getnextToken(itr)!="number"){
            cout<<"Unexpected Token\n";
            numflag=1;
           return NULL; 
        }        
        struct symbol* t= newNode("-"+tk,"number",NULL,NULL);
        consume(tk);
        return t;
    }
    else if(tk=="("){
        consume(tk);        
        struct symbol* node=expression();
        if(peek()!=")"){
           cout<<"Expected )\n";
           numflag=1;
           return NULL; 
        }
        consume(")");
        return node;
    }
    else if(tk=="["){
        sorvorm=1;
        consume(tk);
        struct symbol* node=listexp();
        if(peek()!="]"){
            vecflag=1;
           cout<<"Expected ]\n";
           return NULL;
        }
        consume("]");
        return node;
    }   
    else {
           numflag=1;
           vecflag=1;
           cout<<"Unexpected Token\n";
           return NULL;
        }
}

string getnextToken(list<list<string> >::iterator itr){
    list<list<string> >::iterator nested_list_itr=itr; 
    list<string>::iterator single_list_itr; 
    list<string>& single_list_pointer = *nested_list_itr;  
    single_list_itr=single_list_pointer.begin();
    return *single_list_itr;
}

string getnextTokenValue(list<list<string> >::iterator itr){
    list<list<string> >::iterator nested_list_itr=itr; 
    list<string>::iterator single_list_itr; 
    list<string>& single_list_pointer = *nested_list_itr;  
    single_list_itr=single_list_pointer.begin();
    return *(++single_list_itr);
}

double evaluateTree(struct symbol* node){
    if (node == NULL) 
        return 0; 
    if(node->leftsym==NULL && node->rightsym==NULL){
        return atof(node->tk.value.c_str());
    }
    double l=evaluateTree(node->leftsym); 
    double r=evaluateTree(node->rightsym); 
    if (node->tk.value=="+")  
        return l+r;  
    if (node->tk.value=="-")  
        return l-r;  
    if (node->tk.value=="*"||node->tk.value==".*")  
        return l*r;
    return 0;   
} 

string evaluateVectorTree(struct symbol* node){
     if (node == NULL) 
        return ""; 
    if(node->leftsym==NULL && node->rightsym==NULL){
        return node->tk.value;
    }
    string l=evaluateVectorTree(node->leftsym); 
    string r=evaluateVectorTree(node->rightsym); 
    if (node->tk.value=="+" && ((findtype(l)=="vector" && findtype(r)=="number") || (findtype(l)=="number" && findtype(r)=="vector"))) 
            return VectorNumber(l,r,"+");     
    else if (node->tk.value=="+" && findtype(l)=="vector" && findtype(r)=="vector")  
        return VectorVector(l,r,"+");
    else if (node->tk.value=="+" &&  ((findtype(l)=="matrix"&& findtype(r)=="number")||(findtype(l)=="number"&& findtype(r)=="matrix")))
        return MatrixNumber(l,r,"+"); 
    else if(node->tk.value=="+" && ((findtype(l)=="matrix" && findtype(r)=="vector")||(findtype(l)=="vector"&& findtype(r)=="matrix")))
           return MatrixVector(l,r,"+"); 
    else if (node->tk.value=="+" && findtype(l)=="matrix" && findtype(r)=="matrix")  
        return MatrixMatrix(l,r,"+");

    if (node->tk.value=="-" && ((findtype(l)=="vector" && findtype(r)=="number")||(findtype(l)=="number" && findtype(r)=="vector")))  
        return VectorNumber(l,r,"-"); 
    else if (node->tk.value=="-" && findtype(l)=="vector" && findtype(r)=="vector")  
        return VectorVector(l,r,"-"); 
    else if (node->tk.value=="-" &&  ((findtype(l)=="matrix" && findtype(r)=="number")||(findtype(l)=="number" && findtype(r)=="matrix")))  
        return MatrixNumber(l,r,"-");        
    else if(node->tk.value=="-" && ((findtype(l)=="matrix" && findtype(r)=="vector")||(findtype(l)=="vector" && findtype(r)=="matrix")))
           return MatrixVector(l,r,"-");  
    else if (node->tk.value=="-" && findtype(l)=="matrix" && findtype(r)=="matrix")  
        return MatrixMatrix(l,r,"-");
          
    if((node->tk.value=="*"||node->tk.value==".*") && ((findtype(l)=="vector" && findtype(r)=="number")||(findtype(l)=="number"&& findtype(r)=="vector")))  
        return VectorNumber(l,r,node->tk.value);         
    else if((node->tk.value=="*" || node->tk.value==".*") && (findtype(l)=="vector" && findtype(r)=="vector"))
        return VectorVector(l,r,node->tk.value);    
    else if((node->tk.value=="*"||node->tk.value==".*") &&  ((findtype(l)=="matrix" && findtype(r)=="number")||(findtype(l)=="number"&& findtype(r)=="matrix")))
        return MatrixNumber(l,r,node->tk.value);    
    else if((node->tk.value=="*"||node->tk.value==".*") && ((findtype(l)=="matrix" && findtype(r)=="vector")||(findtype(l)=="vector"&& findtype(r)=="matrix")))
           return MatrixVector(l,r,node->tk.value);  
    else if((node->tk.value==".*"||node->tk.value=="*") && findtype(l)=="matrix" && findtype(r)=="matrix")  
        return MatrixMatrix(l,r,node->tk.value);
    return ""; 
} 

string VectorNumber(string vector,string num,string op){      
    string result="";
    double number;
    if(findtype(vector)=="number"){
        number=atof(vector.c_str());
        if(op=="+"){
            int i=0;
            while(i<num.length()){
                if(num[i]=='['||num[i]==']'||num[i]==','){
                    result+= num[i];    
                    i++;
                    continue;
                }         
                string mid="";
                if(num[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(num[i])){
                    mid+=num[i];
                    i++;
                }                
                if(num[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(num[i])){
                        mid+=num[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())+number);                
            }
            
        }else if(op=="-"){
            int i=0;
            while(i<num.length()){
                if(num[i]=='['||num[i]==']'||num[i]==','){
                    result+= num[i];    
                    i++;
                    continue;
                }         
                string mid="";
                if(num[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(num[i])){
                    mid+=num[i];
                    i++;
                }                
                if(num[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(num[i])){
                        mid+=num[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())-number);                
            }

        }else if(op=="*"||op==".*"){
            int i=0;
            while(i<num.length()){
                if(num[i]=='['||num[i]==']'||num[i]==','){
                    result+= num[i];    
                    i++;
                    continue;
                }         
                string mid="";
                if(num[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(num[i])){
                    mid+=num[i];
                    i++;
                }                
                if(num[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(num[i])){
                        mid+=num[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())*number);                
            }
        }
    }else{
        number=atof(num.c_str());
        if(op=="+"){
            int i=0;
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'||vector[i]==','){
                    result+= vector[i];
                    i++;
                    continue;
                }             
                string mid="";
                if(vector[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(vector[i])){
                    mid+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid+=vector[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())+number);                
            }
        }else if(op=="-"){
            int i=0;
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'||vector[i]==','){
                    result+= vector[i];
                    i++;
                    continue;
                }             
                string mid="";
                if(vector[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(vector[i])){
                    mid+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid+=vector[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())-number);                
            }

        }else if(op=="*"||op==".*"){
            int i=0;
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'||vector[i]==','){
                    result+= vector[i];
                    i++;
                    continue;
                }             
                string mid="";
                if(vector[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(vector[i])){
                    mid+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid+=vector[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())*number);                
            }
        }
    }
    result+="";
    return result;    
}

string VectorVector(string vector1,string vector2,string op){    
            
    struct dem d1=findDem(vector1);
    struct dem d2=findDem(vector2);
    int i=0;
    string result="";
    if(op=="+"){        
        if(d1.x==1 && d1.y==1 && d2.x==1 && d2.y==1){
            string mid1="";
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){                
                    i++;
                    continue;
                }
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                } 
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }                
            }
            i=0;
            string mid2="";
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){                    
                    result+=vector2[i];                        
                    i++;
                    continue;
                }             
               if(vector2[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector2[i])){
                    mid2+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid2+=vector2[i];
                        i++; 
                    }
                }                
                result+=to_string(atof(mid1.c_str())+atof(mid2.c_str()));                
            }
            return result;

        }else if(d1.x==1 && d1.y==1){
            string mid1="";
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    i++;
                    continue;
                }
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }                
            }
            i=0;
            
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){
                    result+=vector2[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector2[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector2[i])){
                    mid2+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid2+=vector2[i];
                        i++; 
                    }
                }
                 result+=to_string(atof(mid1.c_str())+atof(mid2.c_str()));                
            }           
            return result;
        }
        else if(d2.x==1 && d2.y==1){
            string mid1="";
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){
                    i++;
                }
                if(vector2[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(vector2[i])){
                    mid1+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid1+=vector2[i];
                        i++; 
                    }
                }                
            }
            i=0;
            
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    result+=vector1[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector1[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector1[i])){
                    mid2+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid2+=vector1[i];
                        i++; 
                    }
                }
                 result+=to_string(atof(mid2.c_str())+atof(mid1.c_str()));                
            }           
            return result;            
        }
        else{
            if(d1.x!=d2.x || d1.y!=d2.y){        
                cout<<"Demensions Mismatch\n";
                return "";
            }            
            int j=0;
            while(i<vector1.length() || j<vector2.length()){
                string mid1="";
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    i++;
                }                
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }    
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }
                string mid2="";
                if(vector2[j]=='['||vector2[j]==']'||vector2[j]==','){
                    result+=vector2[j];
                    j++;
                }             
                if(vector2[j]=='-'){
                    mid2="-"+mid2;
                    j++;
                }  
                while(isdigit(vector2[j])){
                    mid2+=vector2[j];
                    j++;
                }                
                if(vector2[j]=='.'){  
                    mid2+=".";              
                    j++;
                    while(isdigit(vector2[j])){
                        mid2+=vector2[j];
                        j++; 
                    }
                }
                if(mid1!=""||mid2!="")
                    result+=to_string(atof(mid1.c_str())+atof(mid2.c_str()));
            }
            return result;    
        }

    }else if(op=="-"){
        if(d1.x==1 && d1.y==1 && d2.x==1 && d2.y==1){
            string mid1="";
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){                
                    i++;
                    continue;
                }
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            string mid2="";
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){                    
                    result+=vector2[i];                        
                    i++;
                    continue;
                }             
               if(vector2[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector2[i])){
                    mid2+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid2+=vector2[i];
                        i++; 
                    }
                }                
                result+=to_string(atof(mid1.c_str())-atof(mid2.c_str()));
                
            }
            return result;

        }else if(d1.x==1 && d1.y==1){
            string mid1="";
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    i++;
                }
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){
                    result+=vector2[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector2[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector2[i])){
                    mid2+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid2+=vector2[i];
                        i++; 
                    }
                }
                 result+=to_string(atof(mid1.c_str())-atof(mid2.c_str()));
                
            }
           
            return result;
        }
        else if(d2.x==1 && d2.y==1){
            string mid1="";
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){
                    i++;
                }
                if(vector2[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(vector2[i])){
                    mid1+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid1+=vector2[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    result+=vector1[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector1[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector1[i])){
                    mid2+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid2+=vector1[i];
                        i++; 
                    }
                }
                 result+=to_string(atof(mid2.c_str())-atof(mid1.c_str()));
                
            }
           
            return result;
            
        }
        else{
            if(d1.x!=d2.x || d1.y!=d2.y){        
                cout<<"Demensions Mismatch\n";
                return ""; 
            }
            int j=0;
            while(i<vector1.length() || j<vector2.length()){
                string mid1="";
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    i++;
                }  
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                } 
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }
                string mid2="";
                if(vector2[j]=='['||vector2[j]==']'||vector2[j]==','){
                    result+=vector2[j];
                    j++;
                }             
                if(vector2[j]=='-'){
                    mid2="-"+mid2;
                    j++;
                } 
                while(isdigit(vector2[j])){
                    mid2+=vector2[j];
                    j++;
                }                
                if(vector2[j]=='.'){  
                    mid2+=".";              
                    j++;
                    while(isdigit(vector2[j])){
                        mid2+=vector2[j];
                        j++; 
                    }
                }
                if(mid1!=""||mid2!="")
                    result+=to_string(atof(mid1.c_str())-atof(mid2.c_str()));
                
                
            }
            return result;    
        }


    }else if(op=="*"||op==".*"){
        if(d1.x==1 && d1.y==1 && d2.x==1 && d2.y==1){
            string mid1="";
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){                
                    i++;
                    continue;
                }  
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                } 
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            string mid2="";
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){                    
                    result+=vector2[i];                        
                    i++;
                    continue;
                }             
               if(vector2[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector2[i])){
                    mid2+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid2+=vector2[i];
                        i++; 
                    }
                }                
                result+=to_string(atof(mid1.c_str())*atof(mid2.c_str()));
                
            }
            return result;

        }else if(d1.x==1 && d1.y==1){
            string mid1="";
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    i++;
                    continue;
                }  
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                } 
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){
                    result+=vector2[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector2[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector2[i])){
                    mid2+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid2+=vector2[i];
                        i++; 
                    }
                }
                 result+=to_string(atof(mid1.c_str())*atof(mid2.c_str()));
                
            }
           
            return result;
        }
        else if(d2.x==1 && d2.y==1){
            string mid1="";
            while(i<vector2.length()){
                if(vector2[i]=='['||vector2[i]==']'||vector2[i]==','){
                    i++;
                    continue;
                } 
                if(vector2[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }  
                while(isdigit(vector2[i])){
                    mid1+=vector2[i];
                    i++;
                }                
                if(vector2[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector2[i])){
                        mid1+=vector2[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            
            while(i<vector1.length()){
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    result+=vector1[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector1[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector1[i])){
                    mid2+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid2+=vector1[i];
                        i++; 
                    }
                }
                 result+=to_string(atof(mid2.c_str())*atof(mid1.c_str()));
                
            }
           
            return result;
            
        }
        else{
            if(d1.x!=d2.x || d1.y!=d2.y){        
                cout<<"Demensions Mismatch\n";
                return ""; 
            }
            int j=0;
            while(i<vector1.length() || j<vector2.length()){
                string mid1="";
                if(vector1[i]=='['||vector1[i]==']'||vector1[i]==','){
                    i++;
                }
                if(vector1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(vector1[i])){
                    mid1+=vector1[i];
                    i++;
                }                
                if(vector1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector1[i])){
                        mid1+=vector1[i];
                        i++; 
                    }
                }
                string mid2="";
                if(vector2[j]=='['||vector2[j]==']'||vector2[j]==','){
                    result+=vector2[j];
                    j++;
                }             
                if(vector2[j]=='-'){
                    mid2="-"+mid2;
                    j++;
                } 
                while(isdigit(vector2[j])){
                    mid2+=vector2[j];
                    j++;
                }                
                if(vector2[j]=='.'){  
                    mid2+=".";              
                    j++;
                    while(isdigit(vector2[j])){
                        mid2+=vector2[j];
                        j++; 
                    }
                }
                if(mid1!=""||mid2!="")
                    result+=to_string(atof(mid1.c_str())*atof(mid2.c_str()));
                
                
            }
            return result;    
        }

        
    }
    return ""; 
} 

string MatrixNumber(string matrix,string num,string op){      
    
    string result="";
    double number;
    if(findtype(matrix)=="number"){
        number=atof(matrix.c_str());
        if(op=="+"){
            int i=0;
            while(i<num.length()){
                if(num[i]=='['||num[i]==']'||num[i]==','||matrix[i]==';'){
                    result+= num[i];    
                    i++;                    
                    continue;
                    
                }         
                string mid="";
                if(num[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(num[i])){
                    mid+=num[i];
                    i++;
                }                
                if(num[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(num[i])){
                        mid+=num[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())+number);
                
            }
            

        }else if(op=="-"){
            int i=0;
            while(i<num.length()){
                if(num[i]=='['||num[i]==']'||num[i]==','||matrix[i]==';'){
                    result+= num[i];    
                    i++;                    
                    continue;
                }         
                string mid="";
                if(num[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(num[i])){
                    mid+=num[i];
                    i++;
                }                
                if(num[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(num[i])){
                        mid+=num[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())-number);
                
            }

        }else if(op=="*"||op==".*"){
            int i=0;
            while(i<num.length()){
                if(num[i]=='['||num[i]==']'||num[i]==','||matrix[i]==';'){
                    result+= num[i];    
                    i++;                    
                    continue;
                }         
                string mid="";
                if(num[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(num[i])){
                    mid+=num[i];
                    i++;
                }                
                if(num[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(num[i])){
                        mid+=num[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())*number);
                
            }

        }
    }else{
        number=atof(num.c_str());
        if(op=="+"){
            int i=0;
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'||matrix[i]==','||matrix[i]==';'){
                    result+= matrix[i];
                    i++;
                    continue;
                }             
                string mid="";
                if(matrix[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(matrix[i])){
                    mid+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid+=matrix[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())+number);
            }
            
        

        }else if(op=="-"){
            int i=0;
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'||matrix[i]==','||matrix[i]==';'){
                    result+= matrix[i];
                    i++;                    
                    continue;
                }             
                string mid="";
                if(matrix[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(matrix[i])){
                    mid+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid+=matrix[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())-number);
                
            }

        }else if(op=="*"||op==".*"){
            int i=0;
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'||matrix[i]==','||matrix[i]==';'){
                    result+= matrix[i];
                    i++;                    
                    continue;
                }             
                string mid="";
                if(matrix[i]=='-'){
                    mid="-"+mid;
                    i++;
                } 
                while(isdigit(matrix[i])){
                    mid+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid+=matrix[i];
                        i++; 
                    }
                }
                result+=to_string(atof(mid.c_str())*number);
                
            }

        }
    }
    result+="";
    return result;
    
}

string MatrixVector(string matrix,string vector,string op){
    struct dem d1=findDem(matrix);
    struct dem d2=findDem(vector);
    string result="";
    int i=0;
    if(d1.x==1 && d1.y==1){
        if(op=="+"){ 
            string mid1="";
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'){
                    i++;
                    continue;
                } 
                if(matrix[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }  
                while(isdigit(matrix[i])){
                    mid1+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid1+=matrix[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'||vector[i]==','||vector[i]==';'){
                    result+=vector[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector[i])){
                    mid2+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid2+=vector[i];
                        i++; 
                    }
                }
                    result+=to_string(atof(mid1.c_str())+atof(mid2.c_str()));
                
            }
            return result;
        }
         if(op=="-"){ 
            string mid1="";
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'){
                    i++;
                    continue;
                }
                if(matrix[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(matrix[i])){
                    mid1+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid1+=matrix[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'||vector[i]==','||vector[i]==';'){
                    result+=vector[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector[i])){
                    mid2+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid2+=vector[i];
                        i++; 
                    }
                }
                    result+=to_string(atof(mid1.c_str())-atof(mid2.c_str()));
                
            }
            return result;
        }
         if(op=="*"||op==".*"){ 
            string mid1="";
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'){
                    i++;
                    continue;
                }  
                if(matrix[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                } 
                while(isdigit(matrix[i])){
                    mid1+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid1+=matrix[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'||vector[i]==','||vector[i]==';'){
                    result+=vector[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(vector[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(vector[i])){
                    mid2+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid2+=vector[i];
                        i++; 
                    }
                }
                    result+=to_string(atof(mid1.c_str())*atof(mid2.c_str()));
                
            }
            return result;
        }
    }
    else if(d2.x==1 && d2.y==1){
        if(op=="+"){ 
            string mid1="";
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'){
                    i++;
                    continue;
                }  
                if(vector[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                } 
                while(isdigit(vector[i])){
                    mid1+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid1+=vector[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'||matrix[i]==','||matrix[i]==';'){
                    result+=matrix[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(matrix[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(matrix[i])){
                    mid2+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid2+=matrix[i];
                        i++; 
                    }
                }
                    result+=to_string(atof(mid2.c_str())+atof(mid1.c_str()));
                
            }
            return result;
        }
         if(op=="-"){ 
            string mid1="";
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'){
                    i++;
                    continue;
                }
                if(vector[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(vector[i])){
                    mid1+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid1+=vector[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'||matrix[i]==','||matrix[i]==';'){
                    result+=matrix[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(matrix[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(matrix[i])){
                    mid2+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid2+=matrix[i];
                        i++; 
                    }
                }
                    result+=to_string(atof(mid2.c_str())-atof(mid1.c_str()));
                
            }
            return result;
        }
         if(op=="*"||op==".*"){ 
           string mid1="";
            while(i<vector.length()){
                if(vector[i]=='['||vector[i]==']'){
                    i++;
                    continue;
                } 
                if(vector[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }  
                while(isdigit(vector[i])){
                    mid1+=vector[i];
                    i++;
                }                
                if(vector[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(vector[i])){
                        mid1+=vector[i];
                        i++; 
                    }
                }
                
            }
            i=0;
            while(i<matrix.length()){
                if(matrix[i]=='['||matrix[i]==']'||matrix[i]==','||matrix[i]==';'){
                    result+=matrix[i];
                    i++;
                    continue;
                }             
                string mid2="";
                if(matrix[i]=='-'){
                    mid2="-"+mid2;
                    i++;
                } 
                while(isdigit(matrix[i])){
                    mid2+=matrix[i];
                    i++;
                }                
                if(matrix[i]=='.'){  
                    mid2+=".";              
                    i++;
                    while(isdigit(matrix[i])){
                        mid2+=matrix[i];
                        i++; 
                    }
                }
                    result+=to_string(atof(mid2.c_str())*atof(mid1.c_str()));
                
            }
            return result;
        } 

    }
    else{        
        cout<<"Demensions Mismatch\n";
        return "";
    }
    return "";
}

string MatrixMatrix(string matrix1,string matrix2,string op){
    struct dem d1=findDem(matrix1);
    struct dem d2=findDem(matrix2);
    string result="";
    int i=0;
    int j=0;  
    if(op=="*"){
        if(d1.y!=d2.y){
            cout<<"Demensions Mismatch\n";
            return "";
        }
        return MatrixMult(matrix1,d1.x,d1.y,matrix2);
    }
    else if(op==".*"){
        if(d1.x!=d2.x || d1.y!=d2.y){
            cout<<"Demensions Mismatch\n";
            return "";
        }
         int j=0;
            while(i<matrix1.length() || j<matrix2.length()){
                string mid1="";
                if(matrix1[i]=='['||matrix1[i]==']'||matrix1[i]==','||matrix1[i]==';'){
                    i++;
                }
                if(matrix1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }   
                while(isdigit(matrix1[i])){
                    mid1+=matrix1[i];
                    i++;
                }                
                if(matrix1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(matrix1[i])){
                        mid1+=matrix1[i];
                        i++; 
                    }
                }
                string mid2="";
                if(matrix2[j]=='['||matrix2[j]==']'||matrix2[j]==','||matrix2[j]==';'){
                    result+=matrix2[j];
                    j++;
                }             
                if(matrix2[j]=='-'){
                    mid2="-"+mid2;
                    j++;
                }
                while(isdigit(matrix2[j])){
                    mid2+=matrix2[j];
                    j++;
                }                
                if(matrix2[j]=='.'){  
                    mid2+=".";              
                    j++;
                    while(isdigit(matrix2[j])){
                        mid2+=matrix2[j];
                        j++; 
                    }
                }
                if(mid1!=""||mid2!="")
                    result+=to_string(atof(mid1.c_str())*atof(mid2.c_str()));
                
                
            }
            return result;   

    }
    else{
        if(d1.x!=d2.x || d1.y!=d2.y){
            cout<<"Demensions Mismatch\n";
            return "";
        }
        if(op=="+"){
            while(i<matrix1.length() || j<matrix2.length()){
                string mid1="";
                if(matrix1[i]=='['||matrix1[i]==']'||matrix1[i]==','||matrix1[i]==';'){
                    i++;
                }
                if(matrix1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                }  
                while(isdigit(matrix1[i])){
                    mid1+=matrix1[i];
                    i++;
                }                
                if(matrix1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(matrix1[i])){
                        mid1+=matrix1[i];
                        i++; 
                    }
                }
                string mid2="";
                if(matrix2[j]=='['||matrix2[j]==']'||matrix2[j]==','||matrix2[j]==';'){
                    result+=matrix2[j];
                    j++;
                }             
                if(matrix2[j]=='-'){
                    mid2="-"+mid2;
                    j++;
                }
                while(isdigit(matrix2[j])){
                    mid2+=matrix2[j];
                    j++;
                }                
                if(matrix2[j]=='.'){  
                    mid2+=".";              
                    j++;
                    while(isdigit(matrix2[j])){
                        mid2+=matrix2[j];
                        j++; 
                    }
                }
                if(mid1!=""||mid2!="")
                    result+=to_string(atof(mid1.c_str())+atof(mid2.c_str()));
            }
            return result; 
        }
        else if(op=="-"){
            while(i<matrix1.length() || j<matrix2.length()){
                string mid1="";
                if(matrix1[i]=='['||matrix1[i]==']'||matrix1[i]==','||matrix1[i]==';'){
                    i++;
                } 
                if(matrix1[i]=='-'){
                    mid1="-"+mid1;
                    i++;
                } 
                while(isdigit(matrix1[i])){
                    mid1+=matrix1[i];
                    i++;
                }                
                if(matrix1[i]=='.'){  
                    mid1+=".";              
                    i++;
                    while(isdigit(matrix1[i])){
                        mid1+=matrix1[i];
                        i++; 
                    }
                }
                string mid2="";
                if(matrix2[j]=='['||matrix2[j]==']'||matrix2[j]==','||matrix2[j]==';'){
                    result+=matrix2[j];
                    j++;
                }             
                if(matrix2[j]=='-'){
                    mid2="-"+mid2;
                    j++;
                }
                while(isdigit(matrix2[j])){
                    mid2+=matrix2[j];
                    j++;
                }                
                if(matrix2[j]=='.'){  
                    mid2+=".";              
                    j++;
                    while(isdigit(matrix2[j])){
                        mid2+=matrix2[j];
                        j++; 
                    }
                }
                if(mid1!=""||mid2!="")
                    result+=to_string(atof(mid1.c_str())-atof(mid2.c_str()));
            }
            return result; 
        }
    }
    return "";
}

string MatrixMult(string matrix1,int x,int y,string matrix2){
    
    int i=0,j=0,k=0;
    double arr1[x][y];
    double arr2[x][y];
    double result[x][y];
    while(i<matrix1.length()){
        string num="";
        if(matrix1[i]=='['||matrix1[i]==']'){
            i++;
            continue;
        }
        else if(matrix1[i]==','){
            k++;
            i++;
            continue;
        } 
        else if(matrix1[i]==';'){
            j++;
            k=0;
            i++;
            continue;
        }              
        if(matrix1[i]=='-'){
            num="-"+num;
            i++;
        }
        while(isdigit(matrix1[i])){
            num+=matrix1[i];
            i++;
        }                
        if(matrix1[i]=='.'){  
            num+=".";              
            i++;
            while(isdigit(matrix1[i])){
                num+=matrix1[i];
                i++; 
            }
        }
        arr1[j][k]=atof(num.c_str());       
    }
    i=0,j=0,k=0;
    while(i<matrix2.length()){
            string num="";
            if(matrix2[i]=='['||matrix2[i]==']'){
                i++;
                continue;
            }
            else if(matrix2[i]==','){
                k++;
                i++;
                continue;
            } 
            else if(matrix2[i]==';'){
                j++;
                k=0;
                i++;
                continue;
            }              
            if(matrix2[i]=='-'){
                num="-"+num;
                i++;
            }
            while(isdigit(matrix2[i])){
                num+=matrix2[i];
                i++;
            }                
            if(matrix2[i]=='.'){  
                num+=".";              
                i++;
                while(isdigit(matrix2[i])){
                    num+=matrix2[i];
                    i++; 
                }
            }
            arr2[j][k]=atof(num.c_str());       
        }

     for (i = 0; i < x; i++){ 
        for (j = 0; j < y; j++){ 
            result[i][j] = 0; 
            for (k = 0; k < y; k++) 
                result[i][j] += arr1[i][k] * arr2[k][j]; 
        }  
    }
    string matrix="[";
     for (i = 0; i < x; i++) 
    { 
        for (j = 0; j < y; j++){
            if(j!=y-1) 
                matrix+=to_string(result[i][j])+",";
            else
                matrix+=to_string(result[i][j]);
        }
        if(i!=x-1) 
            matrix+=";"; 
    }
    return matrix+"]";  

}

string findtype(string str){
    
    int i=0;
    while(i<str.length()){
        if(str[i]==';'){
            return "matrix";
        }
        i++;              
    }
    i=0; 
    while(i<str.length()){
        if(str[i]==','||str[i]=='['){
            return "vector";
        }
        i++;  
    } 
    return "number";    
        
        
} 

struct dem findDem(string vector){
    int i=0;
    struct dem d;
    d.x=0;
    d.y=1;
    while (i<vector.length()) {
        if(vector[i]=='['||vector[i]==']'||vector[i]==','){
                i++;
                continue;
        }
        if(vector[i]=='-'){
            i++;
            continue;
        }
        if(vector[i]==';'){
                d.x=0;
                i++;
                d.y++;
                continue;
        } 
        string mid="";
        while(isdigit(vector[i])){
            mid+=vector[i];
            i++;
        }                
        if(vector[i]=='.'){  
            mid+=".";              
            i++;
            while(isdigit(vector[i])){
                mid+=vector[i];
                i++; 
            }
        }
        d.x++;
    }
    return d;
}



