module top(in, a, b, out);
input in, a, b;
output out;
wire n2, n1, en2b, en1b;
nand en1b_ins(en1b, a, b);
not en2b_ins(en2b, a);
_HMUX buf1(n1, in, en1b,a);
buf n2_ins(n2, n1);
_HMUX buf2(out, n2, en2b,b);
endmodule

