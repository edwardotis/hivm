/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2001-2005 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    The tests included in this file were initially taken from the mcpp V2.5
    preprocessor validation suite and were modified to fit into the Boost.Wave 
    unit test requirements.
    The original files of the mcpp preprocessor are distributed under the 
    license reproduced at the end of this file.
=============================================================================*/

// Tests translation limits (number of possible macro definitions should be at 
// least 1024).

#define AA
#define AB
#define AC
#define AD
#define AE
#define AF
#define AG
#define AH
#define AI
#define AJ
#define AK
#define AL
#define AM
#define AN
#define AO
#define AP
#define AQ
#define AR
#define AS
#define AT
#define AU
#define AV
#define AW
#define AX
#define AY
#define AZ
#define Aa
#define Ab
#define Ac
#define Ad
#define Ae
#define Af
#define Ag
#define Ah
#define Ai
#define Aj
#define Ak
#define Al
#define Am
#define An
#define BA
#define BB
#define BC
#define BD
#define BE
#define BF
#define BG
#define BH
#define BI
#define BJ
#define BK
#define BL
#define BM
#define BN
#define BO
#define BP
#define BQ
#define BR
#define BS
#define BT
#define BU
#define BV
#define BW
#define BX
#define BY
#define BZ
#define Ba
#define Bb
#define Bc
#define Bd
#define Be
#define Bf
#define Bg
#define Bh
#define Bi
#define Bj
#define Bk
#define Bl
#define Bm
#define Bn
#define CA
#define CB
#define CC
#define CD
#define CE
#define CF
#define CG
#define CH
#define CI
#define CJ
#define CK
#define CL
#define CM
#define CN
#define CO
#define CP
#define CQ
#define CR
#define CS
#define CT
#define CU
#define CV
#define CW
#define CX
#define CY
#define CZ
#define Ca
#define Cb
#define Cc
#define Cd
#define Ce
#define Cf
#define Cg
#define Ch
#define Ci
#define Cj
#define Ck
#define Cl
#define Cm
#define Cn
#define DA
#define DB
#define DC
#define DD
#define DE
#define DF
#define DG
#define DH
#define DI
#define DJ
#define DK
#define DL
#define DM
#define DN
#define DO
#define DP
#define DQ
#define DR
#define DS
#define DT
#define DU
#define DV
#define DW
#define DX
#define DY
#define DZ
#define Da
#define Db
#define Dc
#define Dd
#define De
#define Df
#define Dg
#define Dh
#define Di
#define Dj
#define Dk
#define Dl
#define Dm
#define Dn
#define EA
#define EB
#define EC
#define ED
#define EE
#define EF
#define EG
#define EH
#define EI
#define EJ
#define EK
#define EL
#define EM
#define EN
#define EO
#define EP
#define EQ
#define ER
#define ES
#define ET
#define EU
#define EV
#define EW
#define EX
#define EY
#define EZ
#define Ea
#define Eb
#define Ec
#define Ed
#define Ee
#define Ef
#define Eg
#define Eh
#define Ei
#define Ej
#define Ek
#define El
#define Em
#define En
#define FA
#define FB
#define FC
#define FD
#define FE
#define FF
#define FG
#define FH
#define FI
#define FJ
#define FK
#define FL
#define FM
#define FN
#define FO
#define FP
#define FQ
#define FR
#define FS
#define FT
#define FU
#define FV
#define FW
#define FX
#define FY
#define FZ
#define Fa
#define Fb
#define Fc
#define Fd
#define Fe
#define Ff
#define Fg
#define Fh
#define Fi
#define Fj
#define Fk
#define Fl
#define Fm
#define Fn
#define GA
#define GB
#define GC
#define GD
#define GE
#define GF
#define GG
#define GH
#define GI
#define GJ
#define GK
#define GL
#define GM
#define GN
#define GO
#define GP
#define GQ
#define GR
#define GS
#define GT
#define GU
#define GV
#define GW
#define GX
#define GY
#define GZ
#define Ga
#define Gb
#define Gc
#define Gd
#define Ge
#define Gf
#define Gg
#define Gh
#define Gi
#define Gj
#define Gk
#define Gl
#define Gm
#define Gn
#define HA
#define HB
#define HC
#define HD
#define HE
#define HF
#define HG
#define HH
#define HI
#define HJ
#define HK
#define HL
#define HM
#define HN
#define HO
#define HP
#define HQ
#define HR
#define HS
#define HT
#define HU
#define HV
#define HW
#define HX
#define HY
#define HZ
#define Ha
#define Hb
#define Hc
#define Hd
#define He
#define Hf
#define Hg
#define Hh
#define Hi
#define Hj
#define Hk
#define Hl
#define Hm
#define Hn
#define IA
#define IB
#define IC
#define ID
#define IE
#define IF
#define IG
#define IH
#define II
#define IJ
#define IK
#define IL
#define IM
#define IN
#define IO
#define IP
#define IQ
#define IR
#define IS
#define IT
#define IU
#define IV
#define IW
#define IX
#define IY
#define IZ
#define Ia
#define Ib
#define Ic
#define Id
#define Ie
#define If
#define Ig
#define Ih
#define Ii
#define Ij
#define Ik
#define Il
#define Im
#define In
#define JA
#define JB
#define JC
#define JD
#define JE
#define JF
#define JG
#define JH
#define JI
#define JJ
#define JK
#define JL
#define JM
#define JN
#define JO
#define JP
#define JQ
#define JR
#define JS
#define JT
#define JU
#define JV
#define JW
#define JX
#define JY
#define JZ
#define Ja
#define Jb
#define Jc
#define Jd
#define Je
#define Jf
#define Jg
#define Jh
#define Ji
#define Jj
#define Jk
#define Jl
#define Jm
#define Jn
#define KA
#define KB
#define KC
#define KD
#define KE
#define KF
#define KG
#define KH
#define KI
#define KJ
#define KK
#define KL
#define KM
#define KN
#define KO
#define KP
#define KQ
#define KR
#define KS
#define KT
#define KU
#define KV
#define KW
#define KX
#define KY
#define KZ
#define Ka
#define Kb
#define Kc
#define Kd
#define Ke
#define Kf
#define Kg
#define Kh
#define Ki
#define Kj
#define Kk
#define Kl
#define Km
#define Kn
#define LA
#define LB
#define LC
#define LD
#define LE
#define LF
#define LG
#define LH
#define LI
#define LJ
#define LK
#define LL
#define LM
#define LN
#define LO
#define LP
#define LQ
#define LR
#define LS
#define LT
#define LU
#define LV
#define LW
#define LX
#define LY
#define LZ
#define La
#define Lb
#define Lc
#define Ld
#define Le
#define Lf
#define Lg
#define Lh
#define Li
#define Lj
#define Lk
#define Ll
#define Lm
#define Ln
#define MA
#define MB
#define MC
#define MD
#define ME
#define MF
#define MG
#define MH
#define MI
#define MJ
#define MK
#define ML
#define MM
#define MN
#define MO
#define MP
#define MQ
#define MR
#define MS
#define MT
#define MU
#define MV
#define MW
#define MX
#define MY
#define MZ
#define Ma
#define Mb
#define Mc
#define Md
#define Me
#define Mf
#define Mg
#define Mh
#define Mi
#define Mj
#define Mk
#define Ml
#define Mm
#define Mn
#define NA
#define NB
#define NC
#define ND
#define NE
#define NF
#define NG
#define NH
#define NI
#define NJ
#define NK
#define NL
#define NM
#define NN
#define NO
#define NP
#define NQ
#define NR
#define NS
#define NT
#define NU
#define NV
#define NW
#define NX
#define NY
#define NZ
#define Na
#define Nb
#define Nc
#define Nd
#define Ne
#define Nf
#define Ng
#define Nh
#define Ni
#define Nj
#define Nk
#define Nl
#define Nm
#define Nn
#define OA
#define OB
#define OC
#define OD
#define OE
#define OF
#define OG
#define OH
#define OI
#define OJ
#define OK
#define OL
#define OM
#define ON
#define OO
#define OP
#define OQ
#define OR
#define OS
#define OT
#define OU
#define OV
#define OW
#define OX
#define OY
#define OZ
#define Oa
#define Ob
#define Oc
#define Od
#define Oe
#define Of
#define Og
#define Oh
#define Oi
#define Oj
#define Ok
#define Ol
#define Om
#define On
#define PA
#define PB
#define PC
#define PD
#define PE
#define PF
#define PG
#define PH
#define PI
#define PJ
#define PK
#define PL
#define PM
#define PN
#define PO
#define PP
#define PQ
#define PR
#define PS
#define PT
#define PU
#define PV
#define PW
#define PX
#define PY
#define PZ
#define Pa
#define Pb
#define Pc
#define Pd
#define Pe
#define Pf
#define Pg
#define Ph
#define Pi
#define Pj
#define Pk
#define Pl
#define Pm
#define Pn
#define QA
#define QB
#define QC
#define QD
#define QE
#define QF
#define QG
#define QH
#define QI
#define QJ
#define QK
#define QL
#define QM
#define QN
#define QO
#define QP
#define QQ
#define QR
#define QS
#define QT
#define QU
#define QV
#define QW
#define QX
#define QY
#define QZ
#define Qa
#define Qb
#define Qc
#define Qd
#define Qe
#define Qf
#define Qg
#define Qh
#define Qi
#define Qj
#define Qk
#define Ql
#define Qm
#define Qn
#define RA
#define RB
#define RC
#define RD
#define RE
#define RF
#define RG
#define RH
#define RI
#define RJ
#define RK
#define RL
#define RM
#define RN
#define RO
#define RP
#define RQ
#define RR
#define RS
#define RT
#define RU
#define RV
#define RW
#define RX
#define RY
#define RZ
#define Ra
#define Rb
#define Rc
#define Rd
#define Re
#define Rf
#define Rg
#define Rh
#define Ri
#define Rj
#define Rk
#define Rl
#define Rm
#define Rn
#define SA
#define SB
#define SC
#define SD
#define SE
#define SF
#define SG
#define SH
#define SI
#define SJ
#define SK
#define SL
#define SM
#define SN
#define SO
#define SP
#define SQ
#define SR
#define SS
#define ST
#define SU
#define SV
#define SW
#define SX
#define SY
#define SZ
#define Sa
#define Sb
#define Sc
#define Sd
#define Se
#define Sf
#define Sg
#define Sh
#define Si
#define Sj
#define Sk
#define Sl
#define Sm
#define Sn
#define TA
#define TB
#define TC
#define TD
#define TE
#define TF
#define TG
#define TH
#define TI
#define TJ
#define TK
#define TL
#define TM
#define TN
#define TO
#define TP
#define TQ
#define TR
#define TS
#define TT
#define TU
#define TV
#define TW
#define TX
#define TY
#define TZ
#define Ta
#define Tb
#define Tc
#define Td
#define Te
#define Tf
#define Tg
#define Th
#define Ti
#define Tj
#define Tk
#define Tl
#define Tm
#define Tn
#define UA
#define UB
#define UC
#define UD
#define UE
#define UF
#define UG
#define UH
#define UI
#define UJ
#define UK
#define UL
#define UM
#define UN
#define UO
#define UP
#define UQ
#define UR
#define US
#define UT
#define UU
#define UV
#define UW
#define UX
#define UY
#define UZ
#define Ua
#define Ub
#define Uc
#define Ud
#define Ue
#define Uf
#define Ug
#define Uh
#define Ui
#define Uj
#define Uk
#define Ul
#define Um
#define Un
#define VA
#define VB
#define VC
#define VD
#define VE
#define VF
#define VG
#define VH
#define VI
#define VJ
#define VK
#define VL
#define VM
#define VN
#define VO
#define VP
#define VQ
#define VR
#define VS
#define VT
#define VU
#define VV
#define VW
#define VX
#define VY
#define VZ
#define Va
#define Vb
#define Vc
#define Vd
#define Ve
#define Vf
#define Vg
#define Vh
#define Vi
#define Vj
#define Vk
#define Vl
#define Vm
#define Vn
#define WA
#define WB
#define WC
#define WD
#define WE
#define WF
#define WG
#define WH
#define WI
#define WJ
#define WK
#define WL
#define WM
#define WN
#define WO
#define WP
#define WQ
#define WR
#define WS
#define WT
#define WU
#define WV
#define WW
#define WX
#define WY
#define WZ
#define Wa
#define Wb
#define Wc
#define Wd
#define We
#define Wf
#define Wg
#define Wh
#define Wi
#define Wj
#define Wk
#define Wl
#define Wm
#define Wn
#define XA
#define XB
#define XC
#define XD
#define XE
#define XF
#define XG
#define XH
#define XI
#define XJ
#define XK
#define XL
#define XM
#define XN
#define XO
#define XP
#define XQ
#define XR
#define XS
#define XT
#define XU
#define XV
#define XW
#define XX
#define XY
#define XZ
#define Xa
#define Xb
#define Xc
#define Xd
#define Xe
#define Xf
#define Xg
#define Xh
#define Xi
#define Xj
#define Xk
#define Xl
#define Xm
#define Xn
#define YA
#define YB
#define YC
#define YD
#define YE
#define YF
#define YG
#define YH
#define YI
#define YJ
#define YK
#define YL
#define YM
#define YN
#define YO
#define YP
#define YQ
#define YR
#define YS
#define YT
#define YU
#define YV
#define YW
#define YX
#define YY
#define YZ
#define Ya
#define Yb
#define Yc
#define Yd
#define Ye
#define Yf
#define Yg
#define Yh
#define Yi
#define Yj
#define Yk
#define Yl
#define Ym
#define Yn
#define ZA
#define ZB
#define ZC
#define ZD
#define ZE
#define ZF
#define ZG
#define ZH
#define ZI
#define ZJ
#define ZK
#define ZL
#define ZM
#define ZN
#define ZO
#define ZP
#define ZQ
#define ZR
#define ZS
#define ZT
#define ZU
#define ZV
#define ZW
#define ZX 1

/*-
 * Copyright (c) 1998, 2002-2005 Kiyoshi Matsui <kmatsui@t3.rim.or.jp>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

