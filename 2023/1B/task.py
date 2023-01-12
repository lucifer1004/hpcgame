#!/usr/bin/python3

import os
import sys

sbatch_file = f'''#!/bin/bash
#SBATCH -o job.%j.out
#SBATCH --partition=compute
#SBATCH -J hpcgame-p1b
#SBATCH --cores=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

./program {sys.argv[1]} > output.dat
'''

with open('task.sh', 'w') as f:
    f.write(sbatch_file)

os.system(f'sbatch task.sh')
os.system('seff "$(cat job_id.dat)" > seff.dat')
