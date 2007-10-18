echo Model Validation using parameters that had best Error Rate from Model Selection phase.

echo APV Model Validation
./hivm -p model-validation -d APV -o results/APV-modelValidation_t2t10   -c 14 -g -13.5  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d APV -o results/APV-modelValidation_t2  -c 15 -g -13.5  -t 2 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d APV -o results/APV-modelValidation_t10 -c 13 -g -12   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

echo ATV Model Validation
./hivm -p model-validation -d ATV -o results/ATV-modelValidation_t2t10 -c 2 -g -3  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d ATV -o results/ATV-modelValidation_t2    -c 15   -g -13.5     -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d ATV -o results/ATV-modelValidation_t10   -c 12.5 -g -2    -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

echo IDV Model Validation
./hivm -p model-validation -d IDV -o results/IDV-modelValidation_t2t10 -c 8.5    -g -13  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d IDV -o results/IDV-modelValidation_t2 	  -c 7    -g -7.5  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d IDV -o results/IDV-modelValidation_t10   -c 1.5  -g -2   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

echo LPV Model Validation
./hivm -p model-validation -d LPV -o results/LPV-modelValidation_t2t10 -c 15    -g -8  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d LPV -o results/LPV-modelValidation_t2 	  -c 13      -g -14  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d LPV -o results/LPV-modelValidation_t10   -c 3     -g -7.5   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42


echo NFV Model Validation
./hivm -p model-validation -d NFV -o results/NFV-modelValidation_t2t10 -c 13.5    -g -14  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d NFV -o results/NFV-modelValidation_t2 	  -c 15    -g -14  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d NFV -o results/NFV-modelValidation_t10   -c 7.5   -g -5.5   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

echo RTV Model Validation
./hivm -p model-validation -d RTV -o results/RTV-modelValidation_t2t10 -c 6    -g -8  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d RTV -o results/RTV-modelValidation_t2 	  -c 14.5   -g -14.5  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d RTV -o results/RTV-modelValidation_t10   -c 9.5    -g -5.5   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

echo SQV Model Validation
./hivm -p model-validation -d SQV -o results/SQV-modelValidation_t2t10 -c 15.5    -g -12.5  -t 2 -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d SQV -o results/SQV-modelValidation_t2 	  -c 6     -g -3  -t 2  -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42
./hivm -p model-validation -d SQV -o results/SQV-modelValidation_t10   -c 12.5  -g -8   -t 10 -w ../../data/PI_wild.seq -f ../../data/PR_2006-05-25_v2.0.tsv --suscep-type clinical --seed 42

