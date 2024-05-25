#include <cucaracha.h>
#include <syscall_adapters.h>
#include <std.h>

const int frames = 18;
char * ascii_art[] = {
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                     %%                                \n               liI:BWz                                 \n              ~1ff\\-{}o8                               \n              klzU]+]?}JQho                            \n                vCtl!()[??_]                           \n          %rf/\\\\1)\\))(}1(1->lx                         \n           Q       J)|)Y)((|[iin                       \n           h/       %-\\L1))))){ii                      \n       k\\c1kz   BCLw? %Y)(|Q1((|~:B                    \n              auk      O<])Cw))}(~>8                   \n               nk      aL|\\\\Y(\\)~[_\"                   \n                QL YbCUCh hUt|\\/Lj|[~W                 \n                ZfBm[    |m W{\\|rOkt}}                 \n               )    dj  oa    t(/(Z(/>c                \n                     hXb     WaB/Y\\|\\\\u                \n                      %)j      Bjxf||\\t1BB             \n                      o\\       hc   B  B tk            \n                    qz          O                      \n                   %             a                     \n                                 b                     \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                 &#                                    \n                  >                                    \n                           #                           \n                 a!ii!l;  B                            \n               %l1>-|Qt\\>;                             \n                ~}>[|Q1i+~:                            \n           jZqvhn!)-uv(i][ii(                          \n           pn  8<xj)|f)))1)!lIk                        \n           hj  %,+j/(()((rrQxvb                        \n       %W( o    f>X|(((((()(z),                        \n             B |QZCC/|((((\\(c(~I8                      \n            L0}B MI_|(((((\\/z||lI                      \n             J)h  }_([(()(()jmb_I                      \n             pWmJUO\\/\\|(()(){]rtlw                     \n            \\pJp{d k((|||)(|rvQv_                      \n           % #JUa   /|\\\\\\/t|\\\\\\jCdM                    \n               Wuq  B;|//|L///\\~/rB                    \n                 #\\x/w(|||/|\\/|vW                      \n                 ({   &1/(C|/Jrf                       \n             WvY         Xmm}X #p                      \n                              k  bB                    \n                               m&                      \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                   wW                                  \n                   w&                                  \n                   m~(|<    C                          \n                 k+_)|t/<in}                           \n               8~))i>trxr{i<8                          \n            Zd0wvi+{(fruii]<lO                         \n            vW OtYx|(tjti)(]i>|                        \n            q% i!0Q|(((((fuY00C{                       \n       B8r uh  <lQC|\\((((()(({u                        \n               8IjQCx((((|(\\\\\\tlW                      \n            LaqQU1(|\\|((|jf((v[!W                      \n            xja +~()(((((((tLnucO                      \n              &Ln(t|(|(()({?|)Xb}                      \n           bhbJYfcj|)((((/t(ttf]W                      \n          %ZZz&) L~\\|\\\\||tvj\\/rYbq                     \n             jLj o}|/rrrr/X/()!BjmL                    \n               %tnn/|rrrrrrq/<\\r)                      \n                vf]*|/jjrrjrrx                         \n            BfJ     ttfff\\/fXM                         \n                         L  &ji                        \n                              u8                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                      X,l-_-!,^                        \n                   W+!!__}\\LUt+-B                      \n                 8)}<ii}_{xJu[>>i                      \n               8ubJ<{v/-~/nC|(>}[]B                    \n               %v!?Ut//((((()(<>!X                     \n               iXfCJC\\/(((((/f\\YXQwx                   \n            Xbv}kCJUt\\(((()((|/}iX&)                   \n              ,?LCCxxf/|()((((/-iYUx                   \n             ~/Opw0n|(((()\\/ncfiI )h                   \n             -{\\/||((((((()/|)/Owap                    \n             -!1|CUz)((()\\i]~>i( \\\\w                   \n          }dawJCnt||/c(((\\//t//\\8                      \n        nkOp |}t///u/(|((()1v)rOZQL                    \n           ZLn///////////\\{]q%||(u                     \n             )/t/////\\//\\[(}(}n                        \n          z[k }t///////zn?(C  %                        \n          %Zfa k[XtzLJ|%B8O nc&                        \n                          B    %                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                      %k                               \n                       O                               \n                        W%     o                       \n                          J!!!lI_C                     \n                        bli)i+\\Cj-~                    \n                      aIi>j>>+/X(_{O                   \n                    !I!itf|li|t\\[}1                    \n                    ~l\\Lj|(([|((i(                     \n                   ^?CJx/|(((((tu|aaa#mJ               \n                 }IjJJf\\Y(((((((iX    Wtb              \n               8iiXJLt(/u\\((()(i,      Ww              \n               !ILJC/uCx\\L1|ttzJZCf  %  U  1JU         \n              :i?//tqkz)(((1!!a   %xQ8                 \n             <!|{[~\\r|((||)+<M    b-                   \n            hl[(uQY|()uut(tCp\\xLLr0                    \n            ,Jk0r(|(//\\|\\hYB8 /t1%                     \n          vapv//t/t/t|)8  qX|\\h  81%                   \n          #cZCJt//(1#dv  /r&  B%                       \n       b)B% >\nBU)o        8uL                         \n              BYk                                      \n             /d                                        \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                        Q                              \n                        MB                             \n                          Y    f                       \n                          8    &                       \n                            8  ;!l%                    \n                            X!+t\\t\\m                   \n                         h0)[{[?(\\(;                   \n                       *j1>{|\\}-(\\Y                    \n                      li_}ft()i_\\iB                    \n                     +i~fjt(|1((|/ftfuOno              \n                   b!~(jj|(((LjIh       YX             \n                  )~?ffj(()(({{          M{            \n                %I!tfjt(ux))\\jnJJJJx%B     /rt         \n               pl<ff/(()mx>     z  %)W                 \n              |!+|?)(((\\Wu11-badBbBjm                  \n            8_-([1Y\\)+q O|*  +/fB cY                   \n           []|((XU[k %    CXx/   Lf8                   \n         Y1())Xohhm B    t(]%}\\    B]&                 \n        ((?qabC8  B     {?    BB                       \n    u1       XLJ/8B        un  n                       \n                 {a                                    \n               %L                                      \n               \\                                       \n              Q                                        \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                x                      \n                                 c                     \n                                 8#                    \n                                  +x+#                 \n                                +fj_/fB                \n                            %<rxnX[1i1C                \n                         djJUUCc(}_~?%                 \n                        &~f/-)\\r}O!x                   \n                      /]_-_-(j]>)|ooo*W\\w              \n                   b{~+_--x|~8   &#toU  cr             \n                 i_?-_-}j|])((/r( B  rQ  M|  %         \n              X?)--_-tv?  Zvp    C\\Z  {M               \n           Mi|)))_-1)}({)>&op*    fd   J%              \n         |tjf)1]\\ vY     j\\upOh   \\d    |O&            \n      C}zjjxfQ   Wvb    c[o% Lu   td                   \n    C|zv<o       (z   B|1      px  {Q                  \n   tjfq          C_  m1q        &/   &B                \n %-\\            Jh8  O]           &B                   \n                 zh    O                               \n                 %{                                    \n                  X                                    \n                 Ma                                    \n                 Ma                                    \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                    &Bdv               \n                                  1jtjfr0              \n                             ~XYYUJJUJu_C              \n                           8x>(ttfff]>+%               \n                        d??-------?#/)                 \n                     Ll?-------]B xJ[nJ&&              \n                  C>--------?}       fZ w/k            \n                ]{{----?-O* t[J       X  (#            \n             }(()(|--_>       8YC     MM%8#            \n          W[fft(())_?)t        i|oL      8X            \n       B<txxjfttwB w)q1x       k! fh         wL        \n     0tCYYY/O       vCtk       bt |Z                   \n   J/JUXdh           voW         c81                   \n O(LuY< %          kj|mp           q0                  \n%#&\\)M             jp )Y            81                 \n WM                ,&Lja                               \n                   b%t/                                \n                     Q                                 \n                    B |                                \n                      L                                \n                      &b                               \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                       BX              \n                                   qBq_t1              \n                               &m|XJUUYJu              \n                            n><<++-_+_-<U              \n                          w{---------~j                \n                       B!--------_+_~+<|jnJY%          \n                    Wu?-?------<MLB        \\%          \n                  *I]((1]-{[]X a/|nh       \\%          \n                al](/\\((()]u1    u ZzO#    J%          \n              h>]frrt(/t)n  n         Qx  %            \n             Y}fUnrrrr(%U]/w8)        |w               \n           r()uUUUnnM(     (Ca8 %    /Q                \n          1rxUUYYub B>     %8MXW_    /                 \n        &r|XUUXL    BO        x(J     pM               \n       wXfUUXY0      Q       <x0       BZ              \n     Xnr\\Jc 8/a     \\L      Cvo                        \n     Wdhk#  q       {     aU}                          \n                    i      #h                          \n                            Jv                         \n                             #}                        \n                               L                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                         (             \n                                       BJ              \n                                       %               \n                                ?     UB               \n                                 Xb*}nnuL              \n                              qu-[))fvu}o              \n                            ,<~+-_---->%               \n                          /+---------~]f||_*|          \n                        _~?--------~%      *a          \n                      <----||]][-+[~      B*8          \n                    M<--)))|)((({   \\cmhB8 WM\nd       \n                   w?(?(rrr//\\|n        LzhB           \n                  h{}[/rrrrrrr\\]po B   Q}              \n                 Wf\\(fUXvzrr1 wXrJCL#%w~               \n                Ozx(fUUUUU/ #_M    #nac%               \n               %nXrjJUUUJ) B *a     x|c&m              \n               XUXruUUUz  B  *d   %YzB                 \n              }YYjjUUUY|         xch                   \n            BxUUUxUYzk)fY      *Um                     \n          LCC nUfzYk  0/       #xY                     \n              Mm     q]         BMu8                   \n                     v             &(%                 \n                                     M                 \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                   %                   \n                            8     W)                   \n                             1#}(txvvj#                \n                            \\xx[<i>>_~c                \n                          0>1-x[------_}Jpa            \n                         n~--?---------_J8n            \n                        a<?-+---?--_---~B&n  Z         \n                        l??-----||||||(( B   %         \n                       t-)1?1||||||||||_tzL            \n                      8+-(||\\||\\/jxrnt)b1Y*            \n                      <]|||/tt|rrxrrrrrJ0XoWq          \n                      ~|\\xrxxxxrxcUzrxv Qp*zCk         \n                     drxxxrxxrrvUYUUnjo *%ffd          \n                     JuxrXxxUUnuUJYYY) BxYd            \n                      &rxUUUYYUnUYUYvWaLd              \n                      8\\uUUUYYUYUUUUYUO                \n                       JUYYYYUJzzUYz  p|1#             \n                      Z1cUULULQUXLO      8Z(           \n                     jdh?wjULCCxbo                     \n                   Wh                                  \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                           Opddw B                     \n                         tj|{(jrrfp#                   \n                        )xrrrrcYXj\\xr8                 \n                      %zYt}}{{f?-----}X                \n                      8?------v?------+&               \n                      t???---------------              \n                      -_------------]]]_+o             \n                      %+--_?-][?]]||||||][&            \n                    Bkd?}[)/|\\||||||||||]?b            \n                     &{[?-{|(|||||||/rjfjt(            \n                        -||(/fjrrjxrxrxrjxfUULv        \n                    BuYUf)trrrrrrrrrjrUYYjr}jfB        \n                    njjm ftxxxXXXUUUUUUYUYYvB          \n                       wt)rxrJUUUUUYUYUUUUCx           \n                          #\\UUUUUUUUUUUUUUJU           \n                          adYUUUUUUUUUUYUUUvtZ         \n                        /1   nUUUUYUUUUUUUd  MkrY      \n                       M   Bf8%xUUUUUUUUYu             \n                           X     mJJYUJ)               \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                      B)jjrr/                          \n                     mxrrrrxxrj               B        \n                     XUUUUYJxrxcJq                     \n                     |ft|+>!]\\->~la                    \n                 MnWa~-----]--n_?-_r                   \n                 M#  J------_-?----?<f                 \n                 (o  C_-___]-_--?---->u                \n             W% mu   w---]---_-__+-_?(-]               \n                  QpxXM<--(((((((((((((?Q              \n                   atJ  >-[|()(((((|((||(_             \n                      1wq(())/rrrrrrxrrrr(k            \n                   adbCxbCu1jrxrrxvrunxrrrrQB          \n                    offr   ?xjjzUUUUzUUJJUXC           \n                       M|fx vxrJUUUUUUUUUUUf           \n                          8f\\|JvUUUUJJUUUUJX/          \n                         B/LZBBkYUUUJJUUJUUJJY\\        \n                      WnxM      LzJUUJCJULLJU   [[%    \n                               [B  OZJQLLQQY           \n                               r        mL             \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                       8                               \n                      _    B                  B        \n                    +xxr)!                             \n                   aUvnnxrr)%                          \n                    pvUJUJUccxL                        \n                     ;~+~~~_]<>-8                      \n               /0#*o*i--------1_l                      \n              1B      +-]--------_}                    \n            *)B        f----------?>                   \n                   XUU/r}-?---?}]?)??I                 \n                 (-     B [-{|((||(|((]q               \n                 BcJ     B&|)|((/trrrrjjx              \n                   Cx LorXUj[-/rjjrrvvnjj/8            \n                   voU0uuo    B xrYUXXUYXUXp           \n                 x     YXt0      B|zYXUUUUUUYZ         \n                        B urw      B#XJJUJJUJUYd       \n                            jfk     br{UYUUUUJUY&%     \n                            8YO     &dh_ %nUYUUUzWdx   \n                          8j8        YYvp    XC        \n                        *-#           M+               \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                       8                               \n                      _    B                  B        \n                    +xxr)!                             \n                   aUvnnxrr)%                          \n                    pvUJUJUccxL                        \n                     ;~+~~~_]<>-8                      \n               /0#*o*i--------1_l                      \n              1B      +-]--------_}                    \n            *)B        f----------?>                   \n                   XUU/r}-?---?}]?)??I                 \n                 (-     B [-{|((||(|((]q               \n                 BcJ     B&|)|((/trrrrjjx              \n                   Cx LorXUj[-/rjjrrvvnjj/8            \n                   voU0uuo    B xrYUXXUYXUXp           \n                 x     YXt0      B|zYXUUUUUUYZ         \n                        B urw      B#XJJUJJUJUYd       \n                            jfk     br{UYUUUUJUY&%     \n                            8YO     &dh_ %nUYUUUzWdx   \n                          8j8        YYvp    XC        \n                        *-#           M+               \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                   B                                   \n                  }                                    \n                cnnj1 B                                \n               %<<+|YYJUuhW                            \n                 _>i)(~><-rp                           \n                da&t[]-(?+--_-L                        \n              &cWwrxqa pL)(}_---lu                     \n              tBi         ~~1)}?--?+X B                \n             Zcnw     BUU(   k!{|[?((()B               \n         z+(BB w    Bt(cz        Y>~](((/j             \n            t&       v/*        n/]q f1(rjjj> B        \n                     r        pkcjr     0u)ffczU       \n                    dd        kmrJ         #CjvJJrh    \n                   &q  B       #Qm            Z{tcCvM  \n                  tW            cOc               &kk  \n                                 zpd                   \n                                 krQ                   \n                                 0Bd                   \n                                 *&?                   \n                                 X8q                   \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                   k                                   \n                 Bz                                    \n              0[]/QO                                   \n             Bm(<~-YJXzCU                              \n              B1_~_]())_[)                             \n                Ur-~--))1-_->B                         \n             %qUYtJC1[~{((((+_+|W                      \n            h\\    O     >-1{1|?__<~%                   \n            k/    Z  M-1m /c>)(\\|-_-?                  \n             O    mXJQ  WLa  Q]-__-_[_}                \n         Mkhh%  hku0    #p    Q1>(_1{{]{)              \n                  cC    (&  Ofk  XW/|()(])~            \n                  OU    m pUp    mB   +)((?)&          \n                  c    Z  vW     ar    Bf)jt([         \n                Wv       BZCm    wo      / t)\\fk       \n                           Cv     zh                   \n                            LZ    B%B                  \n                            0#    BB                   \n                           vQ      v                   \n                          qB       c                   \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n",
"                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                     %%                                \n               liI:BWz                                 \n              ~1ff\\-{}o8                               \n              klzU]+]?}JQho                            \n                vCtl!()[??_]                           \n          %rf/\\\\1)\\))(}1(1->lx                         \n           Q       J)|)Y)((|[iin                       \n           h/       %-\\L1))))){ii                      \n       k\\c1kz   BCLw? %Y)(|Q1((|~:B                    \n              auk      O<])Cw))}(~>8                   \n               nk      aL|\\\\Y(\\)~[_\"                   \n                QL YbCUCh hUt|\\/Lj|[~W                 \n                ZfBm[    |m W{\\|rOkt}}                 \n               )    dj  oa    t(/(Z(/>c                \n                     hXb     WaB/Y\\|\\\\u                \n                      %)j      Bjxf||\\t1BB             \n                      o\\       hc   B  B tk            \n                    qz          O                      \n                   %             a                     \n                                 b                     \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n                                                       \n"
};

typedef struct{
    uint32_t freq;
    uint32_t duration;
    uint32_t delay;
} note_t;

const int song_length = 34;
note_t notes[] = {
    {262, 200, 100}, {262, 200, 100}, {262, 200, 100}, {349, 400, 100}, {440, 200, 200},
    {262, 200, 100}, {262, 200, 100}, {262, 200, 100}, {349, 400, 100}, {440, 200, 200},
    {349, 400, 100}, {349, 200, 100}, {330, 200, 100}, {330, 200, 100}, {294, 200, 100}, {294, 200, 100}, {262, 200, 300},
    {262, 200, 100}, {262, 200, 100}, {262, 200, 100}, {330, 400, 100}, {392, 200, 200},
    {262, 200, 100}, {262, 200, 100}, {262, 200, 100}, {330, 400, 100}, {392, 200, 200},
    {523, 400, 100}, {587, 200, 100}, {523, 200, 100}, {494, 200, 100}, {440, 200, 100}, {392, 200, 100}, {349, 200, 300}
};

void play_la_cucaracha(){
    sys_set_font_size(1);
    for (int i = 0; i < song_length; i++){
        sys_clear_text_buffer();
        printf(ascii_art[i % frames]);
        sys_beep(notes[i].freq, notes[i].duration);
        sys_delay(notes[i].delay);
    }
}
