module top(in, a, b, out);
input in, a, b;
output out;
wire n2, n1, en1, en2b, en1b, out, b, a, in;
nand en1b_ins(en1b, a, b);
not en2b_ins(en2b, a);
not en1_ins(en1, en1b);
_DC buf1(n1, in, en1b);
and iso(n2, n1, en1);
_DC buf2(out, n2, en2b);
endmodule