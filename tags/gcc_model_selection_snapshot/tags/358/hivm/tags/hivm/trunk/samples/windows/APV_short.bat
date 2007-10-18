REM MODEL SELECTION
hivm.exe -p model-selection -d APV -o results/APV-CrossValb_short_t10 -t 10 --seed 42 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 4 -z 2 -l -6 -m -2 -n 2

REM prediction A Cost,Gamma PAIR USED IN CROSS-VALIDATION
hivm.exe -p prediction -d APV -o results/APV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical
