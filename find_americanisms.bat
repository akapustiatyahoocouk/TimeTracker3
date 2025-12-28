rem see http://www.tysto.com/uk-us-spelling-list.html
cut -d, -f2 uk-us.csv > us.txt
c:\cygwin64\bin\find . -name "*.txt" | xargs c:\cygwin64\bin\grep -i -f us.txt -o | grep -v us.txt
del us.txt
