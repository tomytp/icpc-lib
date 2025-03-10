
#let project(
  title: "",
  authors: (),
  body,
) = {
  set document(author: authors, title: title)
  set page(
    paper: "a4",
    flipped: true,
    numbering: "1",
    number-align: center,
    margin: (left: 10mm, right: 10mm, top: 11mm, bottom: 10mm),
    columns: 2,
  )
  
  let body-font = "New Computer Modern"
  let sans-font = "New Computer Modern Sans"
  
  set text(font: body-font, lang: "en", size: 12pt)
  show heading: set text(font: sans-font)
  
  {
    set page(columns: 1)
    align(center)[
      #block(text(font: sans-font, weight: 700, 2.75em, title))
      #v(1em)
      #block(text(font: sans-font, 1.5em, authors.join(", ")))
      #image("misc/baffa.png", width: 36em)
    ]
  }
  
  body
}

// Code block styling - adjusted for two columns
#let code-block(txt, lang: none) = {
  block(
    breakable: true, // Allow code blocks to break across columns
    fill: rgb("#f6f8fa"),
    inset: 6pt,
    radius: 4pt,
    width: 100%,
    raw(txt, lang: lang)
  )
}


// Code block with language
#let import-code(source, lang: "cpp") = {
  code-block(read(source), lang: lang)
}

// Complexity table 
#let complexity-table = table(
  columns: (auto, auto),
  inset: 6pt,
  align: horizon,
  [*$n$*], [*Worst AC Algorithm*],
  [$≤ [10..11]$], [$O(n!), O(n^6)$],
  [$≤ [17..19]$], [$O(2^n × n^2)$],
  [$≤ [18..22]$], [$O(2^n × n)$],
  [$≤ [24..26]$], [$O(2^n)$],
  [$≤ 100$], [$O(n^4)$],
  [$≤ 450$], [$O(n^3)$],
  [$≤ 1.5K$], [$O(n^{2.5})$],
  [$≤ 2.5K$], [$O(n^2 log n)$],
  [$≤ 10K$], [$O(n^2)$],
  [$≤ 200K$], [$O(n^{1.5})$],
  [$≤ 4.5M$], [$O(n log n)$],
  [$≤ 10M$], [$O(n log log n)$],
  [$≤ 100M$], [$O(n), O(log n), O(1)$]
)

// Section break that works with columns
#let section-break = pagebreak(weak: true)


// Function to import files from a section
#let import-files(path, files) = {
  for file in files {
    heading(level: 3, file.name)
    import-code("src/" + path + "/" + file.path)
    v(1em)
  }
}

// Function to process a directory structure recursively
#let process-structure(structure, current-path: "") = {
  // Handle root files first if they exist
  if "root_files" in structure {
    for file in structure.root_files {
      heading(level: 1, file.name)
      import-code("src/" + file.path)
      v(1em)
    }
  }
  
  // Process subdirectories
  for (dir-name, content) in structure {
    if dir-name != "root_files" {
      let path = if current-path == "" {
        content.path
      } else {
        current-path + "/" + content.path
      }
      
      heading(level: 1, content.name)
      
      // Import files in this directory
      if "files" in content {
        import-files(path, content.files)
      }
      
      // Recursively process subdirectories
      for (subdir, subcontent) in content {
        if not (subdir in ("files", "name", "path")) {
          process-structure((subdir: subcontent), current-path: path)
        }
      }
    }
  }
}
