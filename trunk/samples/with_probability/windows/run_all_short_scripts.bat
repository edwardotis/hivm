REM Run all short scripts

REM APV_short.bat
REM MODEL SELECTION
hivm.exe -p model-selection -d APV -o results/APV-CrossValb_short_t10 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 4 -z 2 -l -6 -m -2 -n 2

REM prediction A Cost,Gamma PAIR USED IN CROSS-VALIDATION
hivm.exe -p prediction -d APV -o results/APV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical


REM ATV_short.bat
REM MODEL SELECTION
hivm -p model-selection -d ATV -o results/ATV-CrossValb_short_t10 -t 10  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 6 -z 2 -l -6 -m -2 -n 2

REM prediction A C,G PAIR USED IN CROSS-VALIDATION
hivm -p prediction -d ATV -o results/ATV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical


REM IDV_short.bat
REM MODEL SELECTION
hivm -p model-selection -d IDV -o results/IDV-CrossValb_short_t10  -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 6 -z 2 -l -6 -m -2 -n 2

REM prediction A C,G PAIR USED IN CROSS-VALIDATION
hivm -p prediction -d IDV -o results/IDV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical


REM LPV_short.bat
REM MODEL SELECTION
hivm -p model-selection -d LPV -o results/LPV-CrossValb_short_t10 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 6 -z 2 -l -6 -m -2 -n 2

REM prediction A C,G PAIR USED IN CROSS-VALIDATION
hivm -p prediction -d LPV -o results/LPV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical


REM NFV_short.bat
REM MODEL SELECTION
hivm -p model-selection -d NFV -o results/NFV-CrossValb_short_t10 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 6 -z 2 -l -6 -m -2 -n 2


REM prediction A C,G PAIR USED IN CROSS-VALIDATION
hivm -p prediction -d NFV -o results/NFV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical


REM RTV_short.bat
REM MODEL SELECTION
hivm -p model-selection -d RTV -o results/RTV-CrossValb_short_t10 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 6 -z 2 -l -6 -m -2 -n 2

REM prediction A C,G PAIR USED IN CROSS-VALIDATION
hivm -p prediction -d RTV -o results/RTV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical

REM SQV_short.bat
REM MODEL SELECTION
hivm -p model-selection -d SQV -o results/SQV-CrossValb_short_t10 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 6 -z 2 -l -6 -m -2 -n 2

REM prediction A C,G PAIR USED IN CROSS-VALIDATION
hivm -p prediction -d SQV -o results/SQV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical
