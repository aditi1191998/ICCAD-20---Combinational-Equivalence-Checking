
#include "keywords.h"
#include "functions.h"

bool reserved (char* word)
{
    int i;
    for (i = 0; i < RESERVEDNUM; i++)
        if (strcmp(word, reserved_word[i])==0 || strstr(word, "endmodule")!= NULL)
            return true;
    return false;
}

void print_dd (DdManager *gbm, DdNode   *dd, int n, int pr )
{
    printf("DdManager nodes: %ld | ", Cudd_ReadNodeCount(gbm)); /*Reports the number of live nodes in BDDs and ADDs*/
    printf("DdManager vars: %d | ", Cudd_ReadSize(gbm) ); /*Returns the number of BDD variables in existance*/
    printf("DdManager reorderings: %d | ", Cudd_ReadReorderings(gbm) ); /*Returns the number of times reordering has occurred*/
    printf("DdManager memory: %ld \n", Cudd_ReadMemoryInUse(gbm) ); /*Returns the memory in use by the manager measured in bytes*/
    Cudd_PrintDebug(gbm, dd, n, pr);    // Prints to the standard output a DD and its statistics: number of nodes, number of leaves, number of minterms.
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
        //if(*(c->wires[i]->type) != 'I')
        //{
            printf ("c->wire[%d]->type: %s, ",i, c->wires[i]->type);
            printf ("ID: %d,  ", c->wires[i]->id);
            printf ("name: %s, ", c->wires[i]->name);
            printf ("dc: %d, ", c->wires[i]->dc_n);
            printf ("\nInputs (%d): ", c->wires[i]->inputcount);/*Wire inputs*/
            for(j=0; j<c->wires[i]->inputcount; j++)
                printf ("%d ",c->wires[i]->inputs[j]);

            printf ("\nOutputs (%d): ", c->wires[i]->outputcount);/*Wire outputs*/
            for(j=0; j<c->wires[i]->outputcount; j++)
                printf ("%d ",c->wires[i]->outputs[j]);
            printf ("\n");
        //}
         i++;
            
    }
    printf("*************************** END **************************\n");
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
    //free(vertices);
    //free(arcs);
    return arcs_size == 0;
}

void form_dag(circuit c,module m,DdManager *gbm,DdNode *var,DdNode **bdd,int offset)
{
    char str[10]; 
   /*or ( int o = 0 ; o < m->outputcount ; o++ )
    {
   sprintf(str, "%d", m->inputcount);
   fputs(str,write_file[o]);
   fputs(" ",write_file[o]);

   sprintf(str, "%d", m->outputcount); 
   fputs(str,write_file[o]);
   fputs("\n",write_file[o]);
    }*/
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
        if( *(c->wires[k]->type) != 'I')
        {
            for(j=0; j<c->wires[k]->inputcount; j++)
            {
                arc_connect(arcs,c->wires[k]->inputs[j],c->wires[k]->id,&i);
            }
            arc_connect(arcs,c->wires[k]->id,c->wires[k]->outputs[0],&i);
        }
         k++;
            
    }
    acyclic = topological_sort(arcs, size, order, &sorted);
    /*for(int i=0;i<order;i++)
    {
        if(sorted[i] >= (m->wirecount + m->inputcount + m->outputcount))
        {
         wire w = getWire(sorted[i],c);
        if((*(w->type) != 'I'))
        {
        printf("%d ",sorted[i]);
         }
        }
    }*/
   // printf("\n");
   /* DdManager *gbm; /* Global BDD manager. 
    gbm = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,6000000000);*/

    /*DdNode *var;
  for (int i = (m->inputcount-1); i >= 0; i--) {
      var = Cudd_bddIthVar(gbm,i); 
       Cudd_Ref(var);
    }*/
    int wire_cnt = m->inputcount + m->outputcount + m->wirecount;
    /*printf("wirecount = %d\n",wire_cnt);
    //wire_cnt = 100;
    bdd = (DdNode**)malloc(wire_cnt*sizeof(DdNode*));*/
    k = 0;
    while (k<c->wirecount && c->wires[k] != NULL)
    {
      if((*(c->wires[k]->type) != 'I') && ( (c->wires[k]->outputs[0] >= m->inputcount) && (c->wires[k]->outputs[0]< (m->inputcount+m->outputcount))))
        {
            //printf("%d\n",c->wires[k]->outputs[0]- m->inputcount);
            bdd[c->wires[k]->outputs[0]- m->inputcount] = form_bdd(c->wires[k],m->inputcount,gbm,var,offset,c);
            Cudd_Ref(bdd[c->wires[k]->outputs[0]- m->inputcount]);
            //print_dd (gbm, bdd[w->outputs[0]], 2,4);
        }   
         k++;
    }
    /*for(int i=0;i<order;i++)
    {
        if(sorted[i] >= (m->wirecount + m->inputcount + m->outputcount))
        {
         wire w = getWire(sorted[i],c);
        if((*(w->type) != 'I') && ( (w->outputs[0] >= m->inputcount) && (w->outputs[0]< (m->inputcount+m->outputcount))))
        {
            //printf("%d\n",w->outputs[0]- m->inputcount);
            bdd[w->outputs[0]- m->inputcount] = form_bdd(w,m->inputcount,gbm,var,offset,c);
            Cudd_Ref(bdd[w->outputs[0]- m->inputcount]);
            //print_dd (gbm, bdd[w->outputs[0]], 2,4);
        }   
        }
    }*/
   /* for(int i=0;i<m->outputcount;i++)
    {
        if(i >= (m->inputcount+m->outputcount))
            {
               Cudd_RecursiveDeref(gbm,bdd[i]);
               bdd[i] =  Cudd_ReadOne(gbm);
                //free(bdd[i]);
            }
    }*/
    //printf("--------------------------------------------------\n");
   //dc_case(m,write_file,&sorted,c,order);
    //free(sorted);
    //free(arcs); 
}

wire getWire_out (int out, circuit c)
{
    int i=0;
    while (i<c->wirecount && c->wires[i] != NULL) {
        if (c->wires[i]->outputs[0] == out && ( strcmp("I",(c->wires[i]->type))) ) // If node is found in the circuit
            return c->wires[i];
        i++;
    }
    return 0;
}

DdNode* form_bdd(wire w,int n_in,DdManager *gbm,DdNode *var,int offset,circuit c)
{

//printf("%d\n",w->outputs[0]);
    if(!strcmp(w->type,"&"))
    {
    DdNode* temp;
    DdNode* final;
    if(w->inputs[0] < n_in)
        {
          temp = (&var[w->inputs[0]]);
          Cudd_Ref(temp);
        }
         else if(w->inputs[0] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[0] == 10000)
        {
          
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[0],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
           Cudd_Ref(temp);
        }
        final = temp;

         Cudd_Ref(final);
    for(int i = 1;i<w->inputcount;i++)
    {

        if(w->inputs[i] < n_in)
        {
          temp = (&var[w->inputs[i]]);
          Cudd_Ref(temp);
        }
         else if(w->inputs[i] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[i] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
           Cudd_Ref(temp);
        }
       
        final = Cudd_bddAnd(gbm, final, temp);
         Cudd_Ref(final);
         Cudd_RecursiveDeref(gbm,temp);
    }
    
    return final;
    }

    else if(!strcmp(w->type,"~&"))
    {
   DdNode* temp;
    DdNode* final;
    if(w->inputs[0] < n_in)
        {
          temp = (&var[w->inputs[0]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[0] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[0] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[0],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = temp;
         Cudd_Ref(final);
    for(int i = 1;i<w->inputcount;i++)
    {
        if(w->inputs[i] < n_in)
        {
          temp = (&var[w->inputs[i]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[i] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[i] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = Cudd_bddNand(gbm, final, temp);
         Cudd_Ref(final);
         Cudd_RecursiveDeref(gbm,temp);
    }

    return final;
    }

    else if(!strcmp(w->type,"|"))
    {
    DdNode* temp ;
    DdNode* final;
    if(w->inputs[0] < n_in)
        {
          temp = (&var[w->inputs[0]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[0] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[0] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[0],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = temp;
         Cudd_Ref(final);
    for(int i = 1;i<w->inputcount;i++)
    {
        if(w->inputs[i] < n_in)
        {
          temp = (&var[w->inputs[i]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[i] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[i] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = Cudd_bddOr(gbm, final, temp);
         Cudd_Ref(final);
         Cudd_RecursiveDeref(gbm,temp);
    }
    return final;
    }

    else if(!strcmp(w->type,"~|"))
    {
    DdNode* temp;
    DdNode* final;
    if(w->inputs[0] < n_in)
        {
          temp = (&var[w->inputs[0]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[0] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[0] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[0],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = temp;
         Cudd_Ref(final);
    for(int i = 1;i<w->inputcount;i++)
    {
        if(w->inputs[i] < n_in)
        {
          temp = (&var[w->inputs[i]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[i] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[i] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = Cudd_bddNor(gbm, final, temp);
         Cudd_Ref(final);
         Cudd_RecursiveDeref(gbm,temp);
    }
    return final;
    }

    else if(!strcmp(w->type,"^"))
    {
    DdNode* temp ;
    DdNode* final;
    if(w->inputs[0] < n_in)
        {
          temp = (&var[w->inputs[0]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[0] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[0] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[0],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = temp;
         Cudd_Ref(final);
    for(int i = 1;i<w->inputcount;i++)
    {
        if(w->inputs[i] < n_in)
        {
          temp = (&var[w->inputs[i]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[i] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[i] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = Cudd_bddXor(gbm, final, temp);
         Cudd_Ref(final);
         Cudd_RecursiveDeref(gbm,temp);
    }
    return final;
    }

    else if(!strcmp(w->type,"~^"))
    {
    DdNode* temp;
    DdNode* final;
    if(w->inputs[0] < n_in)
        {
          temp = (&var[w->inputs[0]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[0] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[0] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[0],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = temp;
         Cudd_Ref(final);
    for(int i = 1;i<w->inputcount;i++)
    {
        if(w->inputs[i] < n_in)
        {
          temp = (&var[w->inputs[i]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[i] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[i] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
        final = Cudd_bddXnor(gbm, final, temp);
         Cudd_Ref(final);
         Cudd_RecursiveDeref(gbm,temp);
    }
    return final;
    }

    else if(!strcmp(w->type,"buf"))
    {
    DdNode* temp ;
    int i = 0;
        if(w->inputs[i] < n_in)
        {
          temp = (&var[w->inputs[i]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[i] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[i] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
    return temp;
    }

    else if(!strcmp(w->type,"~"))
    {
      DdNode* final;
        DdNode* temp;
    int i = 0;
        if(w->inputs[i] < n_in)
        {
          temp = (&var[w->inputs[i]]);
           Cudd_Ref(temp);
        }
         else if(w->inputs[i] == 9999)
        {
          temp = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp);
        }
        else if(w->inputs[i] == 10000)
        {
          temp = Cudd_ReadOne(gbm);
          Cudd_Ref(temp);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp);
        }
    final = Cudd_Not(temp);
     Cudd_Ref(final);
     Cudd_RecursiveDeref(gbm,temp);
    return final;
    }
    else if(!strcmp(w->type,"mux"))
    {
        DdNode* temp0;
        DdNode* temp1;
        DdNode* temp2;
        DdNode* final;
        int i = 0;
        if(w->inputs[i] < n_in)
        {
          temp0 = (&var[w->inputs[i]]);
           Cudd_Ref(temp0);
        }
         else if(w->inputs[i] == 9999)
        {
          temp0 = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp0);
        }
        else if(w->inputs[i] == 10000)
        {
          temp0 = Cudd_ReadOne(gbm);
          Cudd_Ref(temp0);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp0 = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp0);
        }
        i = 1;
        if(w->inputs[i] < n_in)
        {
          temp1 = (&var[w->inputs[i]]);
           Cudd_Ref(temp1);
        }
         else if(w->inputs[i] == 9999)
        {
          temp1 = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp1);
        }
        else if(w->inputs[i] == 10000)
        {
          temp1 = Cudd_ReadOne(gbm);
          Cudd_Ref(temp1);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp1 = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp1);
        }
        i = 2;
        if(w->inputs[i] < n_in)
        {
          temp2 = (&var[w->inputs[i]]);
           Cudd_Ref(temp2);
        }
         else if(w->inputs[i] == 9999)
        {
          temp2 = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp2);
        }
        else if(w->inputs[i] == 10000)
        {
          temp2 = Cudd_ReadOne(gbm);
          Cudd_Ref(temp2);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp2 = form_bdd(w_new,n_in,gbm,var,offset,c);
            Cudd_Ref(temp2);
        }
        final = Cudd_bddIte(gbm, temp2, temp1,temp0);
         Cudd_Ref(final);
         Cudd_RecursiveDeref(gbm,temp0);
         Cudd_RecursiveDeref(gbm,temp1);
         Cudd_RecursiveDeref(gbm,temp2);
        return final;
       
    }
    else if(!strcmp(w->type,"dc"))
    {
        DdNode* temp1;
        DdNode* temp0;
        DdNode* final = (DdNode*)malloc(sizeof(DdNode));
        int i = 0;
        if(w->inputs[i] < n_in)
        {
          temp0 = (&var[w->inputs[i]]);
          Cudd_Ref(temp0);
        }
        else if(w->inputs[i] == 9999)
        {
          temp0 = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp0);
        }
        else if(w->inputs[i] == 10000)
        {
          // printf("enter\n");
          temp0 = Cudd_ReadOne(gbm);
          Cudd_Ref(temp0);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp0 = form_bdd(w_new,n_in,gbm,var,offset,c);
           Cudd_Ref(temp0);
        }
        i = 1;
        if(w->inputs[i] < n_in)
        {
          temp1 = (&var[w->inputs[i]]);
          Cudd_Ref(temp1);
        }
         else if(w->inputs[i] == 9999)
        {
         
          temp1 = Cudd_ReadLogicZero(gbm);
          Cudd_Ref(temp1);
        }
        else if(w->inputs[i] == 10000)
        {
         
          temp1 = Cudd_ReadOne(gbm);
          Cudd_Ref(temp1);
        }
        else
        {
           wire w_new = getWire_out(w->inputs[i],c);
           temp1 = form_bdd(w_new,n_in,gbm,var,offset,c);
           Cudd_Ref(temp1);
        }
        final = Cudd_bddIte(gbm,temp1,&var[n_in+(w->dc_n)+offset],temp0); 
        Cudd_Ref(final);
        Cudd_RecursiveDeref(gbm,temp1);
        Cudd_RecursiveDeref(gbm,temp0);
        return final;
    }
  // printf("%d ",w->outputs[0]);
   //DdNode *a = Cudd_BddToAdd(gbm, bdd[w->outputs[0]]);
   //Cudd_PrintMinterm(gbm, a);
    //free(bdd);
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

void form_bool (circuit c)
{
    //char out_bool[c->outputcount][LINESIZE];
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
           // printf ("\n");
        }
         i++;
            
    }
}


int getID (char* name, circuit c)
{
    for(int i=0; i<c->size; i++) {
        if (strcmp(name, c->nodes[i]->name)==0) { // If node is found in the circuit, get its index
           // printf("%d..",c->nodes[i]->id);
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
        if (c->wires[i]->id == id && ( *(c->wires[i]->type) != 'I')) // If node is found in the circuit
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
    n->id = id;         /*Store node id*/
    //printf("%s %d ",n->name,id);
}

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
      w->dc_n = c->dc_n;
      c->dc_n = c->dc_n + 1;

    }
    w->type = strdup(type);     /*Wire type*/
    w->name = strdup(name);     /*Wire name*/
    w->inputcount = 0;          /*Initial number of inputs*/
    w->outputcount = 0;         /*Initial number of outputs*/

    for(i = 0; i < c->inputcount; i++) { /*Circuit primary inputs*/
        if (strcmp(w->name, c->inputs[i])==0) {
            w->primary = true;
        }
    }

   // printf("Creating wire: %s, id: %d\n", w->name, w->id);
}

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



void arc_connect(arc *arcs, unsigned int first, unsigned int second,
        unsigned int *pos)
{
    arcs[*pos].first = first;
    arcs[*pos].second = second;
    (*pos)++;
}





void build_module_circuit (FILE *verilog, module m, circuit c)
{
    int i=0, j=0, in=0, out=0, id=0, index=0;       /*Indexes*/
    char linebuf[LINESIZE], tmpbuf[LINESIZE], buffer[BUFSIZE];  /*Buffer for lines of the verilog code*/
    char *token[TOKENSIZE];                     /*Array to hold tokens for the line*/

    memset(linebuf, 0, sizeof(char) * LINESIZE);
    memset(tmpbuf, 0, sizeof(char) * LINESIZE);
    memset(buffer, 0, sizeof(char) * BUFSIZE);
    memset(token, 0, sizeof(char*) * TOKENSIZE);

    /*Initialization of the circuit*/
    c->inputcount = m->inputcount;          /*Set the number of inputs for the ciruit*/
    c->outputcount = m->outputcount;        /*Set the number of outputs for the circuit*/
    c->wirecount = m->wirecount;                /*Set the number of wires for the circuit*/
    c->gatecount = m->gatecount; 
    c->dc_n = 0;               /*Set the number of gates for the circuit*/
    c->size = c->inputcount+c->outputcount+c->wirecount+c->gatecount;/*Number of inputs, outputs and wires*/
    c->wires = (wire *)calloc((c->wirecount) * 10,sizeof(wire));    /*Allocate a contiguous array to index for every wire*/
    c->nodes = (node *)calloc((c->size) * 5,sizeof(node));      /*Allocate a contiguous array to index for every nodes*/

    for (i=0; i < c->inputcount; i++) {         /*Store the names of inputs*/
        c->inputs[i] = calloc (strlen(m->inputs[i]) + 1, sizeof (char));
        strcpy (c->inputs[i], m->inputs [i]);
        //printf("%s %d ",c->inputs[i],id);
        c->nodes[id] = calloc (1,sizeof(struct node_));
        setNode (c->nodes[id], "input", m->inputs [i], id); /*Set values of a node (type, name, id)*/
        id++;
    }
    for (i=0; i < c->outputcount; i++) {            /*Store the names of outputs*/
        c->outputs[i] = calloc (strlen(m->outputs [i]) + 1, sizeof(char));
        strcpy (c->outputs[i], m->outputs [i]);
        //printf("%s %d ",c->outputs[i],id);
        c->nodes[id] = calloc (1,sizeof(struct node_));
        setNode (c->nodes[id], "output", m->outputs [i], id); /*Set values of a node (type, name, id)*/
        id++;
    }
    for (i=0; i < c->wirecount; i++) {              /*Store the names of wires*/
        c->nodes[id] = calloc (1,sizeof(struct node_));
        setNode (c->nodes[id], "wire", m->wires [i], id);   /*Set values of a node (type, name, id)*/
        id++;
    }
    for (i=0; i < c->gatecount; i++) {              /*Store the names of gates*/
        c->nodes[id] = calloc (1,sizeof(struct node_));
        setNode (c->nodes[id], "gate", m->gates [i], id);   /*Set values of a node (type, name, id)*/
        id++;
    }

    c->nodecount = id;

    while (fgets(linebuf,LINESIZE-1,verilog) != NULL) { /*Create wires: gate_type [gate_name](out_port, in_port...)*/
        char *keyword = NULL;                       /*Keyword from verilog line*/

        i=0;
        in=0;
        memset (buffer,0,sizeof(char) * BUFSIZE);               /*Clear the buffer*/
        strcpy (tmpbuf,linebuf);
        token[0] = strtok(tmpbuf, " (),;");         /*Get 1st keyword from the line*/
        keyword = trim(token[0]);

        if (!reserved (keyword)) continue;      /*Skip any comment lines, empty lines or spaces*/
        if (end_of_module (linebuf)) break;     /*If end of module is reached then break*/
        if (!gate (keyword)) continue;          /*Skip everything but gates*/
       
        strcat (buffer,linebuf);
        
        while (!end_of_line(linebuf)) {         /*Check if the line ends with a ';' character (Multiple lines statement)*/
            if (fgets(linebuf,LINESIZE,verilog) != NULL)    /*Otherwise, append all the following lines until ';' is found*/
                strcat (buffer,linebuf);
        }
         
        token[0] = strtok(buffer, " (),;");         /*Tokenize the line to extract data*/
       // printf("debug1 %s\n",token[0]);
        //printf("i = %d",i);
        while(token[i]!= NULL) {
            i++;
            token[i] = strtok(NULL, " (),;\r\n");
           //printf("%s %d\n",token[i],i);
        }


        /*A. Create wires for all the gate outputs*/
        for(j = 3; j < i; j++) {
            //printf("%s\n",token[j]);
            if (!defined (c,token[j])) { /*If wire is not already defined*/
                c->wires[index] = (wire)calloc(1,sizeof(struct wire_));
                build_wire (c, c->wires[index], "I", token[j]);
                c->wires[index]->outputs[0] = getID (token[1], c);/*1/29/15 assign out to I wires*/
                //printf("%d..",c->wires[index]->outputs[0]);
                c->wires[index]->outputcount = 1;
                index++;
            }
        }

        /*B. Create a wire for the gate*/
        c->wires[index] = (wire)calloc(1,sizeof(struct wire_));
        build_wire (c, c->wires[index], keyword, token[1]);
        /*B1. Assign the gate inputs*/
        in = 0;
        for(j = 3; j < i; j++) {
            //printf("%s",token[j]);
            if(!strcmp(token[j],"1'b0"))
              c->wires[index]->inputs[in] = 9999;
            else if(!strcmp(token[j],"1'b1"))
              c->wires[index]->inputs[in] = 10000;
            else
            c->wires[index]->inputs[in] = getID (token[j], c);
            //printf("%d\n",c->wires[index]->inputs[in]);
            c->wires[index]->inputcount++;
            in++;
        }
        /*B2. Assign the gate output*/
        c->wires[index]->outputs[0] = getID (token[2], c);
        // printf("%d..",c->wires[index]->outputs[0]);
        c->wires[index]->outputcount = 1;
        index++;

        /*C. Create wires for the gate output*/
        if (!defined (c,token[2])) { /*If wire is not already defined*/
            c->wires[index] = (wire)calloc(1,sizeof(struct wire_));
            build_wire (c, c->wires[index], "I", token[2]);
            c->wires[index]->inputs[0] = getID (token[1], c);/*1/29/15 assign in to I wires*/
            c->wires[index]->inputcount = 1;
            index++;
        }

        else { /*If wire is already defined*/
            getWireByName(token[2],c)->inputs[0] = getID (token[1], c);/*Find the wire and attach an input to it (10/30/2015) */
            getWireByName(token[2],c)->inputcount = 1;
        }

        memset(linebuf, 0, sizeof(char) * LINESIZE);
        memset(tmpbuf, 0, sizeof(char) * LINESIZE);
        memset(buffer, 0, sizeof(char) * BUFSIZE);
        memset(token, 0, sizeof(char*) * TOKENSIZE);

    }//End while

    c->wirecount = index;

} /*End build_module_circuit*/



/**
 * Parse a signal vector
 * @param a collection of signals, a collection of tokens, a pointer to the index, a pointer to the number of counts
 */
void parse_signal_vector (char *signal_arr[], char *token[], int *index, int *count)
{
    int v,v1,v2;
    char *sig_vector;                   /*Array to hold tokens for the line*/
    sig_vector = strtok (token[*index],":");        /*Tokenize the vector to extract vector width*/
    v1 = atoi(sig_vector);                  /*Starting index for the vector*/
    sig_vector = strtok (NULL, ":");
    v2 = atoi(sig_vector);              /*Ending index for the vector*/
    (*index)++;                     /*Get vector name from the next token*/
    int wordsize;
    for(v = v2; v <= v1; v++) {             /*Add the vector signals to the array of signals*/
        wordsize = strlen(token[*index]);       /*Size of the string read from the line*/
        signal_arr [*count] = calloc (wordsize + 1, sizeof(char));  /*Allocating memory for signal string*/
        strcpy (signal_arr [*count], token[*index]);    /*Add the signal name to the array of signals*/
        (*count)++;                 /*Update the number of signals in the circuit*/
    }
}

/**
 * Parses a verilog file and extract modules statistical data
 * @param the name of the verilog file, the module object
 */
void build_module (FILE *verilog, module m)
{
    int i=0, j=0;                       /*Indexes*/
    char linebuf[LINESIZE], tmpbuf[LINESIZE], buffer[BUFSIZE];  /*Buffer for lines of the verilog code*/
    char *token[TOKENSIZE];                     /*Array to hold tokens for the line*/
    char *keyword;                      /*Keyword from verilog line*/

    /*Make first pass to count primary inputs, primary outputs, and wires.*/
    while (fgets(linebuf,LINESIZE,verilog) != NULL) {
        i=0;
        strcpy (buffer,"");             /*Clear the buffer*/
        strcpy (tmpbuf,linebuf);
        token[0] = strtok(tmpbuf, " (),;");         /*Get 1st keyword from the line*/
        keyword = trim(token[0]);
        if (!reserved (keyword)) continue;      /*skip comment lines, empty lines or spaces*/

        strcat (buffer,linebuf);
        while (!end_of_line(linebuf)) {         /*Check if the line ends with a ';' character (Multiple lines statement)*/
            if (end_of_module (linebuf)) break;     /*If end of module*/
            if (fgets(linebuf,LINESIZE,verilog) != NULL)    /*Otherwise, append all the following lines*/
                strcat (buffer,linebuf);
        }

        token[0] = strtok(buffer, " (),;");         /*Tokenize the line to extract data*/
        while(token[i]!= NULL) {
            i++;
            token[i] = strtok(NULL, " (),;\r\n");
        }

        if (strcmp(keyword, "module")==0) {     /*MODULES*/
            m->name = calloc (strlen(token[1]) + 1, sizeof(char));  /*Allocating memory for module name string*/
            strcpy (m->name,token[1]);          /*Set module name*/
        }

        else if (strcmp(keyword, "input")==0) { /*INPUTS*/
            for(j = 1; j < i; j++) {            /*Parse all the words in the line*/
                if (signal_vector(token[j]))        /*Handle a vector of signals*/
                    parse_signal_vector (m->inputs, token, &j, &m->inputcount);
                else {                              /*Not a vector of signal*/
                    m->inputs [m->inputcount] = calloc (strlen(token[j]) + 1, sizeof(char));    /*Allocating memory for input string*/
                    strcpy (m->inputs [m->inputcount],token[j]);        /*Add the input name to the array of inputs*/
                    m->inputcount ++;                       /*Update the number of inputs in the circuit*/
                   // fputs(token[j],fptr);
                    //fputs("\n",fptr);
                }
            }
        }

        else if (strcmp(keyword, "output")==0) {    /*OUTPUTS*/
            for(j = 1; j < i; j++) {            /*Parse all the words in the line*/
                if (signal_vector(token[j]))        /*Handle a vector of signals*/
                    parse_signal_vector (m->outputs, token, &j, &m->outputcount);
                else {                              /*Not a vector of signal*/
                    m->outputs [m->outputcount] = calloc (strlen(token[j]) + 1, sizeof(char));  /*Allocating memory for output string*/
                    strcpy (m->outputs [m->outputcount],token[j]);      /*Add the output name to the array of outputs*/
                    m->outputcount ++;                      /*Update the number of outputs in the circuit*/
                }
            }
        }

        else if (strcmp(keyword, "wire")==0) {      /*WIRES*/
            for(j = 1; j < i; j++) {            /*Parse all the words in the line*/
                if (signal_vector(token[j]))        /*Handle a vector of signals*/
                    parse_signal_vector (m->wires, token, &j, &m->wirecount);
                else {                              /*Not a vector of signal*/
                    m->wires [m->wirecount] = calloc (strlen(token[j]) + 1, sizeof(char));  /*Allocating memory for wire string*/
                    strcpy (m->wires [m->wirecount],token[j]);          /*Add the wire name to the array of wires*/
                    m->wirecount ++;                        /*Update the number of wires in the circuit*/
                }
            }
        }

        else if (strcmp(keyword, "reg")==0) {       /*REGS*/
            for(j = 1; j < i; j++) {            /*Parse all the words in the line*/
                if (signal_vector(token[j]))        /*Handle a vector of signals*/
                    parse_signal_vector (m->regs, token, &j, &m->regcount);
                else {                          /*Not a vector of signal*/
                    m->regs [m->regcount] = calloc (strlen(token[j]) + 1, sizeof(char));    /*Allocating memory for reg string*/
                    strcpy (m->regs [m->regcount],token[j]);        /*Add the reg name to the array of regs*/
                    m->regcount ++;                 /*Update the number of regs in the circuit*/
                }
            }
        }

        else if (gate (keyword)) {          /*GATES*/
            m->gates[m->gatecount] = calloc (strlen(token[1]) + 1, sizeof(char)); /*Allocating memory for module name string*/
            strcpy (m->gates [m->gatecount], token[1]);     /*Add the module name to the array of modules*/
            m->gatecount ++;                    /*Update the number of modules in the circuit*/
        }

        else if (end_of_module (linebuf))       /*END OF MODULE*/
        {
           // print_module_summary(m);          /*Print summary of the module*/
            break;                  /*End of the module; break*/
        }
    } // end while

} /*end build_modules*/

void build_module_new (FILE *verilog, module m)
{
    int i=0, j=0;                       /*Indexes*/
    char linebuf[LINESIZE], tmpbuf[LINESIZE], buffer[BUFSIZE];  /*Buffer for lines of the verilog code*/
    char *token[TOKENSIZE];                     /*Array to hold tokens for the line*/
    char *keyword;                      /*Keyword from verilog line*/

    /*Make first pass to count primary inputs, primary outputs, and wires.*/
    while (fgets(linebuf,LINESIZE,verilog) != NULL) {
        i=0;
        strcpy (buffer,"");             /*Clear the buffer*/
        strcpy (tmpbuf,linebuf);
        token[0] = strtok(tmpbuf, " (),;");         /*Get 1st keyword from the line*/
        keyword = trim(token[0]);
        if (!reserved (keyword)) continue;      /*skip comment lines, empty lines or spaces*/

        strcat (buffer,linebuf);
        while (!end_of_line(linebuf)) {         /*Check if the line ends with a ';' character (Multiple lines statement)*/
            if (end_of_module (linebuf)) break;     /*If end of module*/
            if (fgets(linebuf,LINESIZE,verilog) != NULL)    /*Otherwise, append all the following lines*/
                strcat (buffer,linebuf);
        }

        token[0] = strtok(buffer, " (),;");         /*Tokenize the line to extract data*/
        while(token[i]!= NULL) {
            i++;
            token[i] = strtok(NULL, " (),;\r\n");
        }

        if (strcmp(keyword, "wire")==0) {      /*WIRES*/
            for(j = 1; j < i; j++) {            /*Parse all the words in the line*/
                if (signal_vector(token[j]))        /*Handle a vector of signals*/
                    parse_signal_vector (m->wires, token, &j, &m->wirecount);
                else {                              /*Not a vector of signal*/
                    m->wires [m->wirecount] = calloc (strlen(token[j]) + 1, sizeof(char));  /*Allocating memory for wire string*/
                    strcpy (m->wires [m->wirecount],token[j]);          /*Add the wire name to the array of wires*/
                    m->wirecount ++;                        /*Update the number of wires in the circuit*/
                }
            }
        }

        else if (strcmp(keyword, "reg")==0) {       /*REGS*/
            for(j = 1; j < i; j++) {            /*Parse all the words in the line*/
                if (signal_vector(token[j]))        /*Handle a vector of signals*/
                    parse_signal_vector (m->regs, token, &j, &m->regcount);
                else {                          /*Not a vector of signal*/
                    m->regs [m->regcount] = calloc (strlen(token[j]) + 1, sizeof(char));    /*Allocating memory for reg string*/
                    strcpy (m->regs [m->regcount],token[j]);        /*Add the reg name to the array of regs*/
                    m->regcount ++;                 /*Update the number of regs in the circuit*/
                }
            }
        }

        else if (gate (keyword)) {          /*GATES*/
            m->gates[m->gatecount] = calloc (strlen(token[1]) + 1, sizeof(char)); /*Allocating memory for module name string*/
            strcpy (m->gates [m->gatecount], token[1]);     /*Add the module name to the array of modules*/
            m->gatecount ++;                    /*Update the number of modules in the circuit*/
        }

        else if (end_of_module(linebuf))       /*END OF MODULE*/
        {
           // print_module_summary(m);            /*Print summary of the module*/
            break;                  /*End of the module; break*/
        }
    } // end while

}


/**
 * Parses a verilog file and create a circuit object using the module
 * @param the circuit object, the file name
 */

int count_dc(FILE* verilog)
{
  int i=0, j=0;  
  int count = 0;                     /*Indexes*/
    char linebuf[LINESIZE], tmpbuf[LINESIZE], buffer[BUFSIZE];  /*Buffer for lines of the verilog code*/
    char *token[TOKENSIZE];                     /*Array to hold tokens for the line*/
    char *keyword;                      /*Keyword from verilog line*/

    /*Make first pass to count primary inputs, primary outputs, and wires.*/
    while (fgets(linebuf,LINESIZE,verilog) != NULL) {
        i=0;
        strcpy (buffer,"");             /*Clear the buffer*/
        strcpy (tmpbuf,linebuf);
        token[0] = strtok(tmpbuf, " (),;");         /*Get 1st keyword from the line*/
        keyword = trim(token[0]);
        if (!reserved (keyword)) continue;      /*skip comment lines, empty lines or spaces*/

        strcat (buffer,linebuf);
        while (!end_of_line(linebuf)) {         /*Check if the line ends with a ';' character (Multiple lines statement)*/
            if (end_of_module (linebuf)) break;     /*If end of module*/
            if (fgets(linebuf,LINESIZE,verilog) != NULL)    /*Otherwise, append all the following lines*/
                strcat (buffer,linebuf);
        }

        token[0] = strtok(buffer, " (),;");         /*Tokenize the line to extract data*/
        while(token[i]!= NULL) {
            i++;
            token[i] = strtok(NULL, " (),;\r\n");
        }
        if (gate (keyword)) {          /*GATES*/
           if(!strcmp(keyword,"_DC"))  
            count++;             /*Update the number of modules in the circuit*/
        }

        else if (end_of_module(linebuf))       /*END OF MODULE*/
        {
           // print_module_summary(m);            /*Print summary of the module*/
            break;                  /*End of the module; break*/
        }
    } // end while
    return count;
}

void parse_verilog_file (circuit c, char *filename,char *filename_new,char *input_name)
{
    FILE *verilog;           /* Verilog file */
    //printf("%s %s-------------------------------\n",filename,filename_new);
    verilog = fopen(filename, "r");     /* Open Verilog file */
   // name = fopen(input_name,"w");
    if (!verilog) {
        fprintf(stderr,"transfer:  cannot open file \"%s\"\n",filename);
        exit(1);
    }
    DdManager *gbm; /* Global BDD manager. */
    gbm = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,5000000000); 
    /*FILE *info;
    info = fopen("new_stdout", "w+");
    Cudd_SetStdout(gbm,info);
     fclose(info);*/
    module m = (module)calloc(1, sizeof(struct module_));   /*Declare an instance of a module*/
    build_module (verilog, m);         /*Create module object*/ 
    rewind(verilog);                /*Sets the stream position indicator to the beginning of verilog file.*/
    build_module_circuit (verilog, m, c);   /*Create circuit object using the module*/
   // print_circuit_summary (c);
    
    FILE *verilog_new;
    verilog_new = fopen(filename_new, "r");
    module m_temp = (module)calloc(1, sizeof(struct module_));
    int dc_n_next = count_dc(verilog_new);
    //int dc_n_next = 10;
    rewind(verilog_new);
    DdNode *var;
    DdNode **bdd;
    DdNode *dc_temp;
  for (int i = (m->inputcount+c->dc_n+dc_n_next-1); i >= 0; i--) {
      var = Cudd_bddIthVar(gbm,i); 
       Cudd_Ref(var);
   }
   int dc_n_first = c->dc_n;
   int wire_cnt = m->outputcount;
   // printf("wirecount = %d\n",wire_cnt);
    bdd = (DdNode**)malloc(wire_cnt*sizeof(DdNode*));
    form_dag (c, m,gbm,var,bdd,0);
    
    for (int i = 0; i < m->wirecount; i++)
        free (m->wires[i]);

    for (int i = 0; i < m->gatecount; i++)
        free (m->gates[i]);
    for (int i=0; i < c->outputcount; i++)
        free (c->outputs[i]);

    for (int i=0; i < c->inputcount; i++)
        free (c->inputs[i]);

    for (int i=0; i < c->nodecount; i++)
        free (c->nodes[i]);
    free(c->nodes);

    for (int i=0; i < c->wirecount; i++) {
        free (c->wires[i]->name);
        free (c->wires[i]->type);
        free (c->wires[i]);
    }
    free(c->wires);
    m->wirecount = 0;
    m->regcount = 0;
    m->gatecount = 0;
    c->dc_n = 0;
    //FILE *verilog_new;
   // verilog_new = fopen(filename_new, "r");
    if (!verilog_new) {
        fprintf(stderr,"transfer:  cannot open file \"%s\"\n",filename_new);
        exit(1);
    }
    build_module_new (verilog_new,m);
    rewind(verilog_new);
    build_module_circuit (verilog_new, m, c);
   // print_circuit_summary (c);
    DdNode **bdd_new;
/*    for (int i = (m->inputcount+dc_n_first+c->dc_n-1); i >= 0; i--) {
      var = Cudd_bddIthVar(gbm,i); 
       Cudd_Ref(var);
   }*/
    wire_cnt = m->inputcount + m->outputcount + m->wirecount;
   // printf("wirecount = %d\n",wire_cnt);
    //wire_cnt = 100;
    bdd_new = (DdNode**)malloc(wire_cnt*sizeof(DdNode*));
    form_dag(c, m,gbm,var,bdd_new,dc_n_first);
   // printf("debug1\n");
    int flag = 0; 
    int *flag_dc;
  // printf("------------------------%d\n",dc_n_first);
    if(dc_n_first != 0)
    {
        char *input = (char*)malloc((m->inputcount+dc_n_first+dc_n_next)*sizeof(char));
        for(int i=0;i<m->outputcount;i++)
        {
         //printf("%d %d\n",i,m->outputcount);
        DdNode *a = Cudd_bddXor(gbm,bdd[i],bdd_new[i]);
        //printf("exit1\n");
        Cudd_Ref(a);

        DdNode *test = Cudd_ReadLogicZero(gbm);
           Cudd_Ref(test);
        //DdNode* temp3 = Cudd_ReadOne(gbm);
        DdNode *temp1 = Cudd_Cofactor(gbm,a,&var[m->inputcount]);
        Cudd_Ref(temp1);
        DdNode *temp2 = Cudd_Not(&var[m->inputcount]);
        Cudd_Ref(temp2);
        temp2 = Cudd_Cofactor(gbm,a,temp2);
        Cudd_Ref(temp2);
        temp2 = Cudd_bddAnd(gbm,temp1,temp2);
        Cudd_Ref(temp2);
        for(int i=m->inputcount+1;i<=(m->inputcount+dc_n_first-1);i++)
        //int i = 9;
        {
           //printf("%d\n",i);
           temp1 = Cudd_Cofactor(gbm,temp1,&var[i]);
           Cudd_Ref(temp1);
           //Cudd_PrintMinterm(gbm, temp1);
           DdNode *temp = Cudd_Not(&var[i]);
           Cudd_Ref(temp2);
           temp2 = Cudd_Cofactor(gbm,temp2,temp);
           Cudd_Ref(temp2);
           temp2 = Cudd_bddAnd(gbm,temp1,temp2);
           Cudd_Ref(temp2);
          // Cudd_PrintMinterm(gbm, temp2);
         }
         /*i = 10;
           printf("%d\n",i);
           DdNode *temp1 = Cudd_Cofactor(gbm,a,&var[i]);
           Cudd_Ref(temp1);
           //Cudd_PrintMinterm(gbm, temp1);
           DdNode *temp2 = Cudd_Not(&var[i]);
           Cudd_Ref(temp2);
           temp2 = Cudd_Cofactor(gbm,a,temp2);
           Cudd_Ref(temp2);
           temp2 = Cudd_bddAnd(gbm,temp1,temp2);
           Cudd_Ref(temp2);
           //Cudd_PrintMinterm(gbm, temp2);
           temp3 = Cudd_bddAnd(gbm,temp3,temp2);
           Cudd_Ref(temp3);*/
           //Cudd_PrintMinterm(gbm, temp3);
         if(temp2 != test)
         {
           Cudd_PrintMinterm(gbm, temp2);
           flag = 1;
           break;
         }
       }
        //fclose(info);
        //fputs("\n",info);
       // if(i != 0)
      /*  FILE *info;
    info = fopen("new_stdout", "w");
    Cudd_SetStdout(gbm,info);
       Cudd_PrintMinterm(gbm, a);
fclose(info);
      printf("exit2\n");  
    info = fopen("new_stdout", "r");
     //fclose(info);
     //   rewind(info);
    int flag1=0;
   // printf("exit\n");
    if(fscanf(info,"%s",input) == EOF)
      flag1 = 1;
    while( fscanf(info,"%s",input) != EOF)
    {
      //printf("check\n");
        flag1 = 0;
    // printf("%s\n",input);

        for(int i=m->inputcount;i<=(m->inputcount+dc_n_first-1);i++)
        {
           // printf("%c %d",input[i],i);
            if(input[i] != '-')
            {
                flag1 = 1;
                break;
            }
        }
       if(flag1 == 0)
        {
            break;
        }
    }
   fclose(info);
    
       // check(info,cec_check,m->inputcount,dc_n_first,dc_n_next);
      // printf("a %s\n", cec_check);
        if(flag1 == 0)
        {
            flag = 1;
           // i = m->outputcount;
            break;
        }*/
        if(flag == 0)
        printf("EQ\n");
    else
    {
        printf("NEQ\n");
        /*for(int i =0 ;i<m->inputcount;i++)
        {
           // printf("%c",ch);
            if(input[i] == '-')
            input[i] = '1';
            printf("%s %c\n",m->inputs[i],input[i]);
        }*/
       //fclose(info);
    }  
    }
    else if(dc_n_first == 0)
    {
      //printf("enter %d\n",m->outputcount);
      int i;
    for(i=0;i<m->outputcount;i++)
    {
        //DdNode *a = Cudd_bddXor(gbm,bdd[i],bdd_new[i]);
        //print_dd (gbm, a, 2,4);
        if( bdd[i] != bdd_new[i])
        {
            flag = 1;
            break;
        }
    }
    if(flag==0)
        printf("EQ\n");
    else
    {
        printf("NEQ\n");
        DdNode *a = Cudd_bddXor(gbm,bdd[i],bdd_new[i]);
        Cudd_Ref(a);
        Cudd_PrintMinterm(gbm, a);
    }  
    }
    
    
    

    /*for (i = 0; i < m->outputcount; i++)
        free (m->outputs[i]);

    for (i = 0; i < m->inputcount; i++)
        free (m->inputs[i]);

    for (i = 0; i < m->wirecount; i++)
        free (m->wires[i]);

    for (i = 0; i < m->gatecount; i++)
        free (m->gates[i]);

    free (m->name);
    free (m);*/

    fclose(verilog);
    fclose(verilog_new);

}

void check(FILE* file1,char* input, int m,int n,int o)
{
   // file1 = fopen("new_stdout", "r");
        rewind(file1);
        //file1 = fopen("new_stdout", "r");
    int flag=0;
    printf("exit\n");
    while( fscanf(file1,"%s",input) != EOF)
    {
       printf("check\n");
        flag = 0;
    // printf("%s\n",input);

        for(int i=m;i<=(m+n-1);i++)
        {
           // printf("%c %d",input[i],i);
            if(input[i] != '-')
            {
                flag = 1;
                break;
            }
        }
       if(flag == 0)
        {
            //fclose(file1);
            return;
        }
    }
   //fclose(file1);
    input = NULL;
   return;

}


/**
 * Main program
 * @param argc, *argv[]
 */
int main (int argc, char *argv[])
{
    int i;
    if (argc != 3) {
        printf("Usage: ./verilog-parser <verilog_file1.v> <verilog_file2.v>\n"); /* Check for a verilog input file */
        exit(1);
    }
    circuit c = (circuit)calloc(1,sizeof(struct circuit_)); /*Declare an instance of a circuit */
    c->name = strdup(argv[1]);              /*Set circuit name*/
    char s[10];
    parse_verilog_file (c, c->name, argv[2],argv[3]);       /*Parse the verilog file */

    //print_circuit_summary (c);            /*Print summary of the circuit */

    /*Free memory*/
 /*   for (i=0; i < c->outputcount; i++)
        free (c->outputs[i]);

    for (i=0; i < c->inputcount; i++)
        free (c->inputs[i]);

    for (i=0; i < c->nodecount; i++)
        free (c->nodes[i]);
    free(c->nodes);

    for (i=0; i < c->wirecount; i++) {
        free (c->wires[i]->name);
        free (c->wires[i]->type);
        free (c->wires[i]);
    }
    free(c->wires);*/

   // free (c->name);
   // free (c); /*Deallocate memory used by the circuit*/

    return 0;
}









