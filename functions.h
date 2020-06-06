
#ifndef _functions_H
#define _functions_H

#ifdef IDENT_H
static const char* const functions_H_Id =
    "$Id$";
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TOKENSIZE 999	/* Maximum length of a token.*/
#define LINESIZE 9999    /* Maximum length of each input line read.*/
#define BUFSIZE 99999	/* Maximum length of a buffer.*/
#define SIZE 9999
#define INPUT	0
#define AND	"&"
#define NAND "~&"
#define OR	"|"
#define NOR	"~|"
#define XOR	"^"
#define XNOR "~^"
#define BUF	"B"
#define NOT	"~"
#define INV	"-"
#define I	"I"
#define	RESERVEDNUM 109
#define	NO_OUT 0
typedef enum { false, true } bool;
typedef char * string;

struct wire_    {
    int id;		/*Wire ID number*/
    char *name;	/*Name of this wire*/
    char *type; 	/*Type of gate driving this wire*/
    int inputcount;		/*Number of wire inputs*/
    int inputs[LINESIZE];	/*Array of inputs*/
    int outputcount;
    int outputs[LINESIZE];	/*Array of outputs.*/
    bool primary;		/*Primary input flag*/
};
typedef struct wire_ *wire;

struct node_ {
    char type[99];	/*input, output, wire, regs*/
    char name[99];	/*node name*/
    int id;	/*node number*/
};
typedef struct node_ *node;

struct circuit_  {
    wire *wires;					/*Array of all wires */
    node *nodes;					/*Array of nodes*/
    char *name;					/*Name of the circuit. */
    int inputcount, outputcount;	   	/*Count of primary inputs and primary outputs. */
    int gatecount, wirecount, nodecount;				/*Number of wires, (gates)*/
    char *inputs[LINESIZE], *outputs[LINESIZE];	/*List of inputs and outputs in the netlist*/
    int size, id;		        		/*Circuit size and identifier*/
};
typedef struct circuit_ *circuit;

struct module_  {
    char *name;					/*Name of the module*/
    int inputcount, outputcount;	   		/*Count of primary inputs and primary outputs. */
    int wirecount, regcount, gatecount;	        /*Count of wires ,regs, gates*/
    char *inputs [LINESIZE], *outputs [LINESIZE];	/*List of inputs and outputs in the netlist*/
    char *wires [LINESIZE], *regs [LINESIZE]; 		/*List of wires, regs, gates in the netlist*/
    char *gates [LINESIZE];
    int id;
};
typedef struct module_ *module;

typedef struct {
    unsigned int first;
    unsigned int second;
} arc;

typedef struct node {
    char s[LINESIZE];
    struct node * next;
} node_t;


/*PROTOTYPES*/
bool reserved (char* word);
bool gate (char* word);
char* trim (char* source);
bool isFinalOutput (wire w, circuit c);
bool signal_vector (char* word);
bool end_of_module (char* word);
bool end_of_line(char* source);
int convert (char* gate);
void print_module_summary (module m);
void form_bool (circuit c);
void print_circuit_summary (circuit c);
int getID(char* name, circuit c);
void setNode ( node n, char* type, char* name, int number);
void build_wire (circuit c, wire w, char* type, char* name);
wire getWire (int id, circuit c);
wire getWireByName (char* name, circuit c);
bool defined (circuit c, char* name);
int get_max (int value1, int value2);
bool defined (circuit c, char* name);
static unsigned int is_root(const arc *graph, const unsigned int *arcs, unsigned int size,
        unsigned int v);
static unsigned int get_roots(const arc *graph, const unsigned int *arcs, unsigned int size,
        unsigned int order, unsigned int *vertices);
unsigned int topological_sort(const arc *graph, unsigned int size, unsigned int order, 
        unsigned int **sorted);
void arc_connect(arc *arcs, unsigned int first, unsigned int second,
        unsigned int *pos);
void form_dag(circuit c,module m);
void form_expr(unsigned int id,circuit c,int n);
void push(node_t * head, char *str);
void list_print(node_t * head,FILE* fptr);
void initialize(int n);
void concatenate(node_t *a,node_t *b);
void output_print(module m, FILE* fptr);
void dc_case(module m, FILE * fptr,unsigned int **sorted,circuit c,int order,int index);
/*END PROTOTYPES*/

#endif

