import subprocess, os, sys

sys.path.insert(0, os.path.abspath('.'))

# -- Project information -----------------------------------------------------

project = 'EirinFixed'
copyright = '2025, KKoishi_'
author = 'KKoishi_'

# -- General configuration ---------------------------------------------------

extensions = [ "breathe" ]

breathe_projects = {}
breathe_default_project = "EirinFixed"
breathe_projects_source = {
    "EirinFixed": ( "../include/eirin", ["fixed.hpp", "fpmath.hpp", "marco.hpp", "parse.hpp", "ext/papilio_integration.hpp"] )
}

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']


# Check if we're running on Read the Docs' servers
read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

if read_the_docs_build:
    subprocess.call('doxygen', shell=True)
    breathe_projects["EirinFixed"] = "xml"
