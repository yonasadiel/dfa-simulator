# DFA Simulator

## Usage

1. Run the program with dfa as parameter.

    ```
    ./dfa-simulator graph.dfa
    ```

2. Input transition every line.
3. Every input will be followed by the name of current state.
4. Input will be terminated with EOF(Ctrl+Z in Command Line) and returning whether the string is `ACCEPTED` OR `DECLINED`


## Generating DFA

All of characters after `#` in the same line are ignored.

First line contains an integer N<sub>Q</sub>, number of states.

Next N<sub>Q</sub> lines contain Q, the name of states.
It is recomended to give description for every states with `#` comment.
q<sub>i</sub> is in the ith line with 0 &le; i &lt; N<sub>Q</sub>.

Next line contains an integer N<sub>A</sub>,

Next N<sub>A</sub> lines contain A, the transition function (alphabet).
A<sub>i</sub> is in the i-th line with 0 &le; i &lt; N<sub>A</sub>.

Next N<sub>Q</sub> lines contain N<sub>A</sub> of states.
Denote the state in i-th row and j-th column (zero-based) as T<sub>i,j</sub>,
the state q<sub>i</sub> can go to T<sub>i,j</sub> with A<sub>j</sub> transition.
A's in same line are separated by single whitespace.

The next line contains an index of start state.
The start state must be in the Q.
Say the start state is q<sub>0</sub>, then 0 is written.

The next line contains N<sub>F</sub>, number of final states.
Next line contains N<sub>F</sub> of indices that correspond to the list of states.
Say if q<sub>i</sub> is final state, i is written.

Example is in this folder, odd.dfa