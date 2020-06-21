using Documenter

makedocs(
    sitename = "Fate grand rider",
    format = Documenter.HTML(prettyurls=false),
    pages = ["index.md", "project.md", "game.md"]
)

deploydocs(
    repo = "github.com/melonedo/fate-grand-rider.git",
    devbranch = "docs"
)
