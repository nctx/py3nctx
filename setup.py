from setuptools import find_packages
from skbuild import setup

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name="nctx", 
    version="0.1",
    author="Mirco Schoenfeld",
    author_email="mirco.schoenfeld@uni-bayreuth.de",
    description="The 'nctx' package provides functionality to obtain shortest paths and infer shortest-path-based centrality metrics in attributed networks.",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/nctx/py3nctx",
    packages=find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
    test_suite="tests"
)
