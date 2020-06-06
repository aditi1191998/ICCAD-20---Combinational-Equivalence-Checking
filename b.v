module top(in, a, b, out);
input in, a, b;
output out;
wire n2, n1, en2b, en1b, out, b, a, in;
nand en1b_ins(en1b, a, b);
not en2b_ins(en2b, a);
_DC buf1(n1, in, en1b);
buf n2_ins(n2, n1);
_DC buf2(out, n2, en2b);
endmodule
