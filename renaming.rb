puts("Missing Argument for class name list") unless ARGV.length == 1

classes = File.read(ARGV[0]).split("\n")
files = `find Source -name "*.[hm]"`.split("\n")`

prefixBlacklistCharacters = "\"k"
postfixWhitelistCharacters = ". ;,()"

files.each do |file|
  classes.each do |className|
    `sed -i '' -E 's/([^#{prefixBlacklistCharacters}]|^)(#{className})([#{postfixWhitelistCharacters}]|$)/\\1LL\\2\\3/g' #{file}`
  end
end
