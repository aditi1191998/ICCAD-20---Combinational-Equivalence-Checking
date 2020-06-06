
#ifdef IDENT_C
static const char* const functions_c_Id =
    "$Id$";
#endif

#include "keywords.h"
#include "functions.h"
node_t * final_exp[99999];

void initialize(int n)
{
	node_t * pInt = NULL; 
for (int i=n;i<99999;i++)
{
	//printf("%s",final_exp[i]->s); 
   final_exp[i] = pInt;
  // final_exp[i] = (node_t *) malloc(sizeof(node_t));
  // strcpy(final_exp[i]->s,"00");
  // final_exp[i]->next = NULL;
}
}
/**
 * Determines if a string is reserved keyword
 * @param the string to compare
 * @return whether the string is reserved or not
 */
bool reserved (char* word)
{
    int i;
    for (i = 0; i < RESERVEDNUM; i++)
        if (strcmp(word, reserved_word[i])==0 || strstr(word, "endmodule")!= NULL)
            return true;
    return false;
}

/**
 * Determines if a string is gate
 * @param the string to check
 * @return whether the string is a gate or not
 */
bool gate (char* word)
{
    int i;
    for (i = 0; i < GATESNUM; i++)
        if (strcmp(word, gate_name[i])==0)
            return true;
    return false;
}

/**
 * Determines if a wire is an output
 * @param the string to check
 * @return whether the string is a an ouput or not
 */
bool isFinalOutput (wire w, circuit c)
{
    int i;
    for(i = 0; i < c->outputcount; i++)
        if (strcmp(w->name, c->outputs[i])==0)
            return true;
    return false;
}

/**
 * Parses a string and removes all character after a numeric character
 * @param the source string
 */
char* trim (char* source)
{
    int i=0, index=0;
    int sr_length = strlen(source);
    for(i=0; i<sr_length; i++)
    {
        if((source[i]=='x' && i!=0)) {
            source[index] = '\0';
            break;
        }
        else
            source[index++] = source[i];
    }
    source[index] = '\0';
    return source;
}

/**
 * Determines if a string is a vector of signals
 * @param the string to check
 * @return whether the string is a vector of signals or not
 */
bool signal_vector (char* word)
{
    int i;
    for (i = 0; i < strlen(word); i++)
        if (word[i] == ':')
            return true;
    return false;
}

/**
 * Determines if end of module
 * @param the string to check
 * @return if end of module or not
 */
bool end_of_module (char* word)
{
    if (strstr(word, "endmodule")!= NULL)
        return true;
    return false;
}

/**
 * Parses a string and search for the character ';'
 * @param the source string
 * @return whether the character ';' is found or not
 */
bool end_of_line(char* source)
{
    char * pchar = strchr(source, ';');
    return (pchar == NULL) ? false : true;
}

/**
 * Convert a gate name to an integer value
 * @param the string gate name
 * @return the gate integer value
 */
int convert (char* gate)
{
    if (strcasecmp(gate, "INPUT")==0)
        return 0;
    else if (strcasecmp(gate, "AND")==0)
        return 1;
    else if (strcasecmp(gate, "NAND")==0)
        return 2;
    else if (strcasecmp(gate, "OR")==0)
        return 3;
    else if (strcasecmp(gate, "NOR")==0)
        return 4;
    else if (strcasecmp(gate, "XOR")==0)
        return 5;
    else if (strcasecmp(gate, "XNOR")==0)
        return 6;
    else if (strcasecmp(gate, "BUF")==0)
        return 7;
    else if (strcasecmp(gate, "NOT")==0 || strcasecmp(gate, "INV")==0)
        return 8;
    else if (strcasecmp(gate, "I")==0)
        return 9;
    else if (strcasecmp(gate, "_HMUX")==0)
        return 10;
    else if (strcasecmp(gate, "_DC")==0)
        return 11;
    else
        return 12;
}

/**
 * Prints the summary of a module - Statistical information
 * @param the module object
 */
void print_module_summary (module m)
{
    int i;
    printf("\n************** Module %s statistical results *************\n", m->name);
    printf("Number of inputs: %d\n", m->inputcount);
    //for(i = 0; i < m->inputcount; i++)
      //  printf("%s ", m->inputs[i]);

    printf("\n\nNumber of outputs: %d\n", m->outputcount);
    //for(i = 0; i < m->outputcount; i++)
      //  printf("%s ", m->outputs[i]);

    printf("\n\nNumber of gates: %d\n", m->gatecount);
    //for(i = 0; i < m->gatecount; i++)
      //  printf("%s ", m->gates[i]);

    printf("\n\nNumber of wires: %d\n", m->wirecount);
    //for(i = 0; i < m->wirecount; i++)
      //  printf("%s ", m->wires[i]);

    printf("\n\nNumber of regs: %d\n", m->regcount);
    //for(i = 0; i < m->regcount; i++)
      //  printf("%s ", m->regs[i]);
    printf("*************************** END **************************\n");
}

/**
 * Prints the summary of a circuit - Statistical information
 * @param the circuit object
 */
void print_circuit_summary (circuit c)
{
    int i,j,row,col;
    printf("\n************** Circuit %s statistical results *************\n", c->name);
    printf("Circuit size: %d\n", c->size);
    printf("Number of primary inputs: %d\n", c->inputcount);
    for(i = 0; i < c->inputcount; i++)
        printf("%s ", c->inputs[i]);

    printf("\n\nNumber of outputs: %d\n", c->outputcount);
    for(i = 0; i < c->outputcount; i++)
        printf("%s ", c->outputs[i]);

    printf("\n\nNumber of gates: %d\n", c->gatecount);

    printf("\n\n");
    i=0;

    while (i<c->wirecount && c->wires[i] != NULL) {
        if(*(c->wires[i]->type) != 'I')
        {
            printf ("c->wire[%d]->type: %s, ",i, c->wires[i]->type);
            printf ("ID: %d,  ", c->wires[i]->id);
            printf ("name: %s, ", c->wires[i]->name);

            printf ("\nInputs (%d): ", c->wires[i]->inputcount);/*Wire inputs*/
            for(j=0; j<c->wires[i]->inputcount; j++)
                printf ("%d ",c->wires[i]->inputs[j]);

            printf ("\nOutputs (%d): ", c->wires[i]->outputcount);/*Wire outputs*/
            for(j=0; j<c->wires[i]->outputcount; j++)
                printf ("%d ",c->wires[i]->outputs[j]);
            printf ("\n");
        }
         i++;
            
    }
    printf("*************************** END **************************\n");
}

void form_dag(circuit c,module m,FILE * write_file)
{
   initialize(0);
 /*  FILE* write_file;
   write_file = fopen("write_output", "w");   
       if (!write_file) {
        fprintf(stderr,"transfer:  cannot open file \"%s\"\n","write_output");
        exit(1);
    }*/
   fputs("Number of primary inputs:", write_file);
   char str[10]; 
   sprintf(str, "%d", m->inputcount); 
   fputs(str,write_file);
   fputs("\n",write_file);
   fputs("Number of primary outputs:", write_file);
   sprintf(str, "%d", m->outputcount); 
   fputs(str,write_file);
   fputs("\n",write_file);
   int n = m->inputcount + m->outputcount + m->wirecount + m->gatecount;
   int n_arc = 0;
   int l = 0;
   while (l<c->wirecount && c->wires[l] != NULL) {
        if(*(c->wires[l]->type) != 'I')
        {
            n_arc = n_arc + c->wires[l]->inputcount + 1;
        }
         l++;
            
    }
   const unsigned int size = n_arc; /* Arcs */
   const unsigned int order = n; /* Vertices */
    arc *arcs = malloc(size * sizeof(arc));
    unsigned int i = 0;
    unsigned int *sorted;
    unsigned int acyclic;
    int k,j;
    k = 0;
    while (k<c->wirecount && c->wires[k] != NULL) {
        if(*(c->wires[k]->type) != 'I')
        {
            //printf ("c->wire[%d]->type: %s, \n",k, c->wires[k]->type);

            for(j=0; j<c->wires[k]->inputcount; j++)
            {
                arc_connect(arcs,c->wires[k]->inputs[j],c->wires[k]->id,&i);
               // printf ("%d %d %u\n",c->wires[k]->inputs[j],c->wires[k]->id, *&i);
            }
            arc_connect(arcs,c->wires[k]->id,c->wires[k]->outputs[0],&i);
        }
         k++;
            
    }
 
    acyclic = topological_sort(arcs, size, order, &sorted);
    printf("Graph is acyclic: %u\n", acyclic);
   dc_case(m,write_file,&sorted,c,order,-1);
    /*for (i = 0; i < order; i++) {
        //printf("%d\n",sorted[i]);
        if(sorted[i]<(n-(m->gatecount)));
           // printf("-----------\n");
    	else 
    	{
        form_expr(sorted[i],c);	
    	}
    }
  output_print(m,write_file);*/
   /*for(int a=m->inputcount; a<(m->inputcount+m->outputcount);a++)
    {
    	printf("%d = ",a);
        char str[10]; 
        sprintf(str, "%d", a); 
        fputs("O",write_file);
        fputs(str,write_file);
        fputs("=",write_file);
    	list_print(final_exp[a],write_file);
        fputs("\n",write_file);
    	printf("\n");
    }
    putchar('\n');*/
 
    free(sorted);
    free(arcs); 
}
void dc_case(module m, FILE * fptr,unsigned int **sorted,circuit c,int order,int index)
{
    initialize(index+1);
    //printf("enter\n");
    int n = m->inputcount + m->outputcount + m->wirecount + m->gatecount;
    for (int i = index+1; i < order; i++) {
        //printf("%d\n",sorted[i]);
        if((*sorted)[i]<(n-(m->gatecount)));
           // printf("-----------\n");
        else 
        {
        wire w;
        w = getWire((*sorted)[i],c);

        if(!strcmp(w->type,"dc")) 
        {
            //printf("%d\n",i);
            form_expr((*sorted)[i],c,1); 
            dc_case(m,fptr,&(*sorted),c,order,i);
            form_expr((*sorted)[i],c,2); 
            dc_case(m,fptr,&(*sorted),c,order,i);
            form_expr((*sorted)[i],c,3); 
            dc_case(m,fptr,&(*sorted),c,order,i);
            form_expr((*sorted)[i],c,4); 
            dc_case(m,fptr,&(*sorted),c,order,i);
            return;
        }
        else
        {
        //printf("enter 1 %d\n",(*sorted)[i]);
        form_expr((*sorted)[i],c,0); 
        }
        }
    }
    output_print(m,fptr);

}
void output_print(module m, FILE* fptr)
{
for(int a=m->inputcount; a<(m->inputcount+m->outputcount);a++)
    {
        printf("%d = ",a);
        char str[10]; 
        sprintf(str, "%d", a); 
        fputs("O",fptr);
        fputs(str,fptr);
        fputs("=",fptr);
        list_print(final_exp[a],fptr);
        fputs("\n",fptr);
        printf("\n");
    }
    putchar('\n');
}

void form_expr(unsigned int id,circuit c,int dc_n)
{
	wire w;
	w = getWire(id,c);
    final_exp[w->outputs[0]] = malloc(sizeof(node_t));
    strcpy(final_exp[w->outputs[0]]->s,"(");
    final_exp[w->outputs[0]]->next = NULL;
    if(w->inputcount == 1)
    { 
      push(final_exp[w->outputs[0]],w->type);
      push(final_exp[w->outputs[0]],"(");
      if(final_exp[w->inputs[0]] == NULL)
     // if(!(strcmp(final_exp[w->inputs[0]]->s,"00")))
            	{
                    char s[10];
                    sprintf(s,"%d",w->inputs[0]);
            		push(final_exp[w->outputs[0]],s);

            	}
            	else
                {
                node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[0]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
                while(current_out->next != NULL) {
                     current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
                }
                push(final_exp[w->outputs[0]],")");
        }
        else if(!strcmp(w->type,"mux"))
        {
            push(final_exp[w->outputs[0]],"(");
            int j = 0;
            if(final_exp[w->inputs[0]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[j]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[0]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],"&"); 
            push(final_exp[w->outputs[0]],"(");
            push(final_exp[w->outputs[0]],"~");  
            if(final_exp[w->inputs[2]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[2]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[2]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],"))"); 
            push(final_exp[w->outputs[0]],"|"); 
            push(final_exp[w->outputs[0]],"(");
            if(final_exp[w->inputs[1]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[1]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[1]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],"&");

            if(final_exp[w->inputs[2]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[2]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[2]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],")");
        }

        else if(!strcmp(w->type,"dc"))
        {
            if(dc_n == 1)
            {
             push(final_exp[w->outputs[0]],"(");
            int j = 0;
            if(final_exp[w->inputs[0]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[j]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[0]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],"&"); 
            push(final_exp[w->outputs[0]],"(");
            push(final_exp[w->outputs[0]],"~");  
            if(final_exp[w->inputs[1]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[1]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[1]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],"))"); 
            push(final_exp[w->outputs[0]],"|"); 
            //push(final_exp[w->outputs[0]],"(");
            if(final_exp[w->inputs[1]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[1]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[1]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],")");
            }
            else if(dc_n == 2)
            {
              //push(final_exp[w->outputs[0]],"(");
            int j = 0;
            if(final_exp[w->inputs[0]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[j]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[0]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],"&"); 
            push(final_exp[w->outputs[0]],"(");
            push(final_exp[w->outputs[0]],"~");  
            if(final_exp[w->inputs[1]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[1]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[1]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],")"); 
            }
            else if(dc_n == 3)
            {
              //push(final_exp[w->outputs[0]],"(");
            int j = 0;
            if(final_exp[w->inputs[0]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[j]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[0]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            push(final_exp[w->outputs[0]],"^");  
            if(final_exp[w->inputs[1]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[1]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[1]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            //push(final_exp[w->outputs[0]],"))"); 
            }
            else
            {
                          int j = 0;
            if(final_exp[w->inputs[0]] == NULL)
            {
                char s[10];
                sprintf(s,"%d",w->inputs[j]);
                push(final_exp[w->outputs[0]],s);
            }
            else
            {
               node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[0]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
            }
            }
        }
    
    else
    {
     int f = (w->inputcount) - 1;   //
     //printf("f %d\n",f);
     int j = 0;
    for(j=0; j<f+1; j++)
            {
                if(final_exp[w->inputs[j]] == NULL)
            	{
                    char s[10];
                    sprintf(s,"%d",w->inputs[j]);
            		 push(final_exp[w->outputs[0]],s);
            	}
                else
                {
                node_t *newHead = malloc(sizeof(node_t));
                node_t *current;
                node_t *current_out;
                current = final_exp[w->inputs[j]];
                current_out = final_exp[w->outputs[0]];
                strcpy(newHead->s, current->s);
               while(current_out->next != NULL) {
                 //   printf("kkkkkkk");
                       current_out = current_out->next;
                }
                current_out->next = newHead;
// Part 3 - the rest of the list
                node_t *p = newHead;
                current = current->next;
                while(current != NULL) {
                    p->next = malloc(sizeof(node_t));
                        p=p->next;
                        strcpy(p->s, current->s);
                       current = current->next;
                }
                p->next = NULL; 
                }
                if(j != f)
            	push(final_exp[w->outputs[0]],w->type);	
            }
    }
    push(final_exp[w->outputs[0]],")");
    //printf("final");
    //list_print(final_exp[w->outputs[0]]);
    //printf("\n");
    return;
}

void concatenate(node_t *a,node_t *b)
{
    if(a != NULL && b != NULL)
    {
        if(a->next == NULL)
            a->next = b;
        else
            concatenate(a->next,b); 
    }
}


void push(node_t * head, char *str) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    /* now we can add a new variable */

    current->next = malloc(sizeof(node_t));
    strcpy(current->next->s,str);
    
    current->next->next = NULL;
}

void list_print(node_t * head, FILE* fptr) {
    node_t * current = head;

    while (current != NULL) {
        printf("%s ", current->s);
        if(!strcmp(current->s,"(") || !strcmp(current->s,"(") || !strcmp(current->s,")") || !strcmp(current->s,"&") || !strcmp(current->s,"~&") || !strcmp(current->s,"|") || !strcmp(current->s,"~|") || !strcmp(current->s,"buf") || !strcmp(current->s,"^") || !strcmp(current->s,"~^") || !strcmp(current->s,"~"))
        fputs(current->s,fptr);
        else
        {
           fputs("I",fptr);
           fputs(current->s,fptr);
        }
        current = current->next;
    }
}


void form_bool (circuit c)
{
    char out_bool[c->outputcount][LINESIZE];
    int i=0;
    while (i<c->wirecount && c->wires[i] != NULL) {
        if(*(c->wires[i]->type) != 'I')
        {
            if(c->wires[i]->outputs[0] >= (c->inputcount + c->outputcount));
            printf ("c->wire[%d]->type: %s, ",i, c->wires[i]->type);
            printf ("ID: %d,  ", c->wires[i]->id);
            printf ("name: %s, ", c->wires[i]->name);

            printf ("\nInputs (%d): ", c->wires[i]->inputcount);/*Wire inputs*/
            for(int j=0; j<c->wires[i]->inputcount; j++)
                printf ("%d ",c->wires[i]->inputs[j]);

            printf ("\nOutputs (%d): ", c->wires[i]->outputcount);/*Wire outputs*/
            for(int j=0; j<c->wires[i]->outputcount; j++)
                printf ("%d ",c->wires[i]->outputs[j]);
            printf ("\n");
        }
         i++;
            
    }
}


int getID (char* name, circuit c)
{
    int i;
    for(i=0; i<c->size; i++) {
        if (strcmp(name, c->nodes[i]->name)==0) { // If node is found in the circuit, get its index
            return c->nodes[i]->id;
        }
    }
    return 0;
}

/**
 * Get a wire by id
 * @param the signal name, the circuit's name
 */
wire getWire (int id, circuit c)
{
    int i=0;
    while (i<c->wirecount && c->wires[i] != NULL) {
        if (c->wires[i]->id == id) // If node is found in the circuit
            return c->wires[i];
        i++;
    }
    return 0;
}

/**
 * Get a wire by name
 * @param the signal name, the circuit's name
 */
wire getWireByName (char* name, circuit c)
{
    int i=0;
    while (c->wires[i] != NULL) {
        if (strcmp(name, c->wires[i]->name)==0) // If node is found in the circuit
            return c->wires[i];
        i++;
    }
    return 0;
}


/**
 * Set values of a node
 * @param the node object, the type of node, the name of the node, the node id
 */
void setNode ( node n, char* type, char* name, int id)
{
    strcpy(n->type, type);
    strcpy(n->name, name);
    n->id = id;			/*Store node id*/
}

/*void setNode ( node n, char* type, char* name, int id)
{
	if(!strcmp("and",type))
	{
      strcpy(n->type, "&");
	}
    strcpy(n->name, name);
    n->id = id;			/*Store node id*/
//}

/**
 * Create a wire
 * @param the circuit object, the wire object, the wire type, the wire name
 */
void build_wire (circuit c, wire w, char* type, char* name)
{
    int i;
    w->id = getID (name, c);   /*Wire ID*/
    if(!strcmp("and",type))
	{
      strcpy(type, "&");	
	}
	else if(!strcmp("nand",type))
	{
      strcpy(type, "~&");
	}
	else if(!strcmp("or",type))
	{
      strcpy(type, "|");
	}
	else if(!strcmp("nor",type))
	{
      strcpy(type, "~|");
	}
	else if(!strcmp("xor",type))
	{
      strcpy(type, "^");
    }
	else if(!strcmp("xnor",type))
	{
      strcpy(type, "~^");
	}
	else if(!strcmp("buf",type))
	{
      strcpy(type, "buf");
	}
	else if(!strcmp("not",type))
	{
      strcpy(type, "~");
	}
    else if(!strcmp("_HMUX",type))
    {
      strcpy(type, "mux");
    }
    else if(!strcmp("_DC",type))
    {
      strcpy(type, "dc");
    }
    w->type = strdup(type);		/*Wire type*/
    w->name = strdup(name);		/*Wire name*/
    w->inputcount = 0;			/*Initial number of inputs*/
    w->outputcount = 0;			/*Initial number of outputs*/

    for(i = 0; i < c->inputcount; i++) { /*Circuit primary inputs*/
        if (strcmp(w->name, c->inputs[i])==0) {
            w->primary = true;
        }
    }

   // printf("Creating wire: %s, id: %d\n", w->name, w->id);
}

/**
 * Determines if a wire is already created
 * @param the circuit object, the wire name
 * @return whether the wire is already created or not
 */
bool defined (circuit c, char* name)
{
    int i=0;
    while (c->wires[i] != NULL) {
        if (strcmp(c->wires[i]->name, name)==0)
            return true;
        i++;
    }
    return false;
}

static unsigned int is_root(const arc *graph, const unsigned int *arcs, unsigned int size,
        unsigned int v)
{
    unsigned int a, root = 1;
    for (a = 0; a < size && root; a++) {
        root = !arcs[a] || graph[a].second != v;
    }
    return root;
}
 
/* Get the vertices with no incoming arcs */
static unsigned int get_roots(const arc *graph, const unsigned int *arcs, unsigned int size,
        unsigned int order, unsigned int *vertices)
{
    unsigned int v, vertices_size = 0;
    for (v = 0; v < order; v++) {
        if (is_root(graph, arcs, size, v)) {
            vertices[v] = 1;
            vertices_size++;
        }
    }
    return vertices_size;
}

unsigned int topological_sort(const arc *graph, unsigned int size, unsigned int order, 
        unsigned int **sorted)
{
    unsigned int *vertices = calloc(order, sizeof(unsigned int));
    unsigned int *arcs = malloc(size * sizeof(unsigned int));
    *sorted = malloc(order * sizeof(unsigned int));
    unsigned int v, a, vertices_size, sorted_size = 0,
            arcs_size = size;
    if (!(vertices && arcs && *sorted)) {
        free(vertices);
        free(arcs);
        free(*sorted);
        *sorted = NULL;
        return 0;
    }
    /* All arcs start off in the graph */
    for (a = 0; a < size; a++) {
        arcs[a] = 1;
    }
    /* Get the vertices with no incoming edges */
    vertices_size = get_roots(graph, arcs, size, order, vertices);
    /* Main loop */
    while (vertices_size > 0) {
        /* Get first vertex */
        for (v = 0; vertices[v] != 1; v++);
        /* Remove from vertex set */
        vertices[v] = 0;
        vertices_size--;
        /* Add it to the sorted array */
        (*sorted)[sorted_size++] = v;
        /* Remove all arcs connecting it to its neighbours */
        for (a = 0; a < size; a++) {
            if (arcs[a] && graph[a].first == v) {
                arcs[a] = 0;
                arcs_size--;
                /* Check if neighbour is now a root */
                if (is_root(graph, arcs, size, graph[a].second)) {
                    /* Add it to set of vertices */
                    vertices[graph[a].second] = 1;
                    vertices_size++;
                } 
            }
        }
    }
    free(vertices);
    free(arcs);
    return arcs_size == 0;
}

void arc_connect(arc *arcs, unsigned int first, unsigned int second,
        unsigned int *pos)
{
    arcs[*pos].first = first;
    arcs[*pos].second = second;
    (*pos)++;
}

