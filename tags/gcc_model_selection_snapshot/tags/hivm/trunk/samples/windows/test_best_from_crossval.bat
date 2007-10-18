REM APV prediction
hivm -p prediction -d APV -o results/APV-predictionBest_t2t10   -c 15.5 -g -14.5  -t 2 -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d APV -o results/APV-predictionBest_t2  -c 14.5 -g -13  -t 2 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d APV -o results/APV-predictionBest_t10 -c 15 -g -8  -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


REM ATV prediction
hivm -p prediction -d ATV -o results/ATV-predictionBest_t2t10 -c 4.5 -g -5  -t 2 -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d ATV -o results/ATV-predictionBest_t2    -c 3   -g -0.5     -t 2  -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d ATV -o results/ATV-predictionBest_t10   -c 1.5 -g -1.5    -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


REM IDV prediction
hivm -p prediction -d IDV -o results/IDV-predictionBest_t2t10 -c 7    -g -9  -t 2 -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d IDV -o results/IDV-predictionBest_t2 	  -c 8    -g -8.5  -t 2  -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d IDV -o results/IDV-predictionBest_t10   -c 0.5  -g -2.5   -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


REM LPV prediction
hivm -p prediction -d LPV -o results/LPV-predictionBest_t2t10 -c 11.5    -g -8  -t 2 -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d LPV -o results/LPV-predictionBest_t2 	  -c 15      -g -14.5  -t 2  -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d LPV -o results/LPV-predictionBest_t10   -c 3.5     -g -2.5   -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


REM NFV prediction
hivm -p prediction -d NFV -o results/NFV-predictionBest_t2t10 -c 15    -g -15  -t 2 -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d NFV -o results/NFV-predictionBest_t2 	  -c 14    -g -7.5  -t 2  -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d NFV -o results/NFV-predictionBest_t10   -c 7.5   -g -5.5   -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

REM RTV prediction
hivm -p prediction -d RTV -o results/RTV-predictionBest_t2t10 -c 7.5    -g -8  -t 2 -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d RTV -o results/RTV-predictionBest_t2 	  -c 11.5   -g -11.5  -t 2  -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d RTV -o results/RTV-predictionBest_t10   -c 9.5    -g -10.5   -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

REM SQV prediction
hivm -p prediction -d SQV -o results/SQV-predictionBest_t2t10 -c 12    -g -9.5  -t 2 -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d SQV -o results/SQV-predictionBest_t2 	  -c 6     -g -3  -t 2  -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
hivm -p prediction -d SQV -o results/SQV-predictionBest_t10   -c 10.5  -g -7.5   -t 10 -w ../data/PI_wild.seq -f ../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
