REM APV model-validation --probability 0
REM hivm -p model-validation --probability 0 -d APV -o results/APV-model-validationBest_t2t10   -c 15.5 -g -14.5  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d APV -o results/APV-model-validationBest_t2  -c 14.5 -g -13  -t 2 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d APV -o results/APV-model-validationBest_t10 13.5 -g -12  -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


REM ATV model-validation --probability 0
REM hivm -p model-validation --probability 0 -d ATV -o results/ATV-model-validationBest_t2t10 -c 4.5 -g -5  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d ATV -o results/ATV-model-validationBest_t2    -c 1   -g -0.5     -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d ATV -o results/ATV-model-validationBest_t10   -c 1.5 -g -1.5    -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


REM IDV model-validation --probability 0
REM hivm -p model-validation --probability 0 -d IDV -o results/IDV-model-validationBest_t2t10 -c 7    -g -9  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d IDV -o results/IDV-model-validationBest_t2 	  -c 8    -g -8.5  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d IDV -o results/IDV-model-validationBest_t10   -c 0.5  -g -2.5   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


REM LPV model-validation --probability 0
REM hivm -p model-validation --probability 0 -d LPV -o results/LPV-model-validationBest_t2t10 -c 11.5    -g -8  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d LPV -o results/LPV-model-validationBest_t2 	  -c 15      -g -14.5  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d LPV -o results/LPV-model-validationBest_t10    -c 3     -g -5.5  -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


REM NFV model-validation --probability 0
REM hivm -p model-validation --probability 0 -d NFV -o results/NFV-model-validationBest_t2t10 -c 15    -g -15  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d NFV -o results/NFV-model-validationBest_t2 	  -c 15.5    -g -13.5  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d NFV -o results/NFV-model-validationBest_t10   -c 7.5   -g -5.5   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

REM RTV model-validation --probability 0
REM hivm -p model-validation --probability 0 -d RTV -o results/RTV-model-validationBest_t2t10 -c 7.5    -g -8  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d RTV -o results/RTV-model-validationBest_t2 	  -c 11.5   -g -11.5  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d RTV -o results/RTV-model-validationBest_t10   -c 9.5    -g -10.5   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

REM SQV model-validation --probability 0
REM hivm -p model-validation --probability 0 -d SQV -o results/SQV-model-validationBest_t2t10 -c 12    -g -9.5  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d SQV -o results/SQV-model-validationBest_t2 	  -c 6     -g -3  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p model-validation --probability 0 -d SQV -o results/SQV-model-validationBest_t10   -c 10.5  -g -7.5   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
