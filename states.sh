SERIAL="$1" 
DATA_FILE="$2"  

while IFS= read -r line; do
    parts=($line)
    if [[ "${parts[0]}" == "$SERIAL" ]]; then
        echo "${parts[1]}" 
        exit 0
    fi
done < "$DATA_FILE"

echo "0"
exit 0
