# HPTS
High-Performance Task Scheduler (C++)

# Idea
Main idea of scheduler is to
- Take tasks
- Split into threads
- Execute them parallery
- Check performance

# How to build
g++ -std=c++14 ./source/main.cpp ./source/core/scheduler.cpp ./source/core/thread_pool.cpp -o HTPS.exe
