hivm -p model-selection -d SQV -o results/SQV-CrossValb_t2t10 -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x -16  -y 16 -z .5 -l -16 -m 16 -n .5
hivm -p model-selection -d SQV -o results/SQV-CrossValbt2 -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x -16  -y 16 -z .5 -l -16 -m 16 -n .5
hivm -p model-selection -d SQV -o results/SQV-CrossValbt10 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical -x -16  -y 16 -z .5 -l -16 -m 16 -n .5
