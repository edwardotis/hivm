REM MODEL SELECTION
hivm -p model-selection -d RTV -o results/RTV-CrossValb_short_t10 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 6 -z 2 -l -6 -m -2 -n 2

REM prediction A C,G PAIR USED IN CROSS-VALIDATION
hivm -p prediction -d RTV -o results/RTV-prediction_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical
