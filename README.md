```markdown
# 🍝 Philosophers Project 🧠

## Overview
Welcome to my Philosophers project! This implementation tackles the classic dining philosophers problem, exploring the intricacies of threads and mutex synchronization in C.

After dedicating significant time to this project, I'm happy to share this clean, efficient solution that earned a perfect **100/100** score in the 42 evaluation. My code is designed to be approachable and well-structured, making it an excellent resource for anyone learning about concurrent programming concepts.

## Features
- ✅ No memory leaks
- ✅ No data races
- ✅ Clean, aligned output formatting
- ✅ Thoroughly tested and validated

## Usage
```bash
# Compile the program
make

# Run with arguments
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Example
```bash
./philo 5 800 200 200 7
```

## Output Format
The program displays status messages with perfectly aligned numbers for better readability:
```
[timestamp in ms] [philosopher number] [action]
```

## Learning Resource
If you're learning about thread synchronization, mutex locks, or concurrent programming in C, feel free to use this code as a reference. I've designed it to be clear and straightforward while properly implementing all the required synchronization mechanisms.

Happy philosophizing! 🤔🍴
```
