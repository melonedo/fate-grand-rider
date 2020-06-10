using Documenter

makedocs(
    sitename = "Fate grand rider",
    format = Documenter.HTML(),
)

deploydocs(
    repo = "github.com/melonedo/fate-grand-rider.git",
    devbranch = "docs"
)
