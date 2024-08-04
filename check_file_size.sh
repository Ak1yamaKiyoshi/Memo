du -h "$1" | cut -f1 | sed 's/G/ GB/' | sed 's/M/ MB/' | sed 's/K/ KB/'
