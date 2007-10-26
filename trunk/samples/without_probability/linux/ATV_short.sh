#MODEL SELECTION
./hivm -p model-selection --probability 0 -d ATV -o results/ATV-CrossValb_short_t10 -t 10  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x 0  -y 4 -z 2 -l -6 -m -2 -n 2

#prediction --probability 0 a Cost,Gamma PAIR USED IN CROSS-VALIDATION
./hivm -p prediction --probability 0 -d ATV -o results/ATV-prediction --probability 0_short_t10 -c 2 -g -4 -t 10 --seed 42 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical

