module top(in, a, b,c, out);
input a, b,c;
output out;
wire d,e,f;
and inst (d,a,b);
_DC buf2 (e, d, b);
not inst1 (f,e);
_DC buf3 (out, f, b);
endmodule
