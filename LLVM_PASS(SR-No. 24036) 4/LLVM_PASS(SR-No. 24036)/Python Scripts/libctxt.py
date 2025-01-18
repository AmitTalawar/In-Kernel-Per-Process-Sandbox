import requests
from bs4 import BeautifulSoup

# Set the version of libc
url = "https://sourceware.org/glibc/manual/2.39/html_node/Function-Index.html"

response = requests.get(url)
soup = BeautifulSoup(response.content, 'html.parser')

functions = []
for code in soup.find_all('code'):
    func_name = code.get_text().strip()
    functions.append(func_name)

uniqueID = 10001 #The ID that dummysyscall will take as an identifier for a particular libc function call

with open("Python Scripts/libc_functions.txt", 'w') as f:
    for func in functions:
        f.write(func + ' ' + str(uniqueID) + '\n')
        uniqueID += 1

print(f"Extracted {len(functions)} functions and written to 'libc_functions.txt'.")
