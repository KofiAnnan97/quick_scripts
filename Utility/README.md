# Bash Scripts

## code_lines.sh
A simple bash script that counts the approximate number of lines of code that are present within a repository. 

### Support parameters
- `-d` desired directory

- `-i` comma delimited string for ignored directories (i.e. "build,target")

```bash
./code_lines.sh -d path/to/desired_dir -i "ignore_1,ignore_2"
```
