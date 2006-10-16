var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_down2 = [
'license.htm',
'faq.htm',
'install.htm',
'getstarted.cpp.htm',
'introduction.htm',
'ad.htm',
'adfun.htm',
'library.htm',
'example.htm',
'appendix.htm'
];
var list_down1 = [
'theory.htm',
'speed.htm',
'adolc.htm',
'fadbad.htm',
'glossary.htm',
'bib.htm',
'bugs.htm',
'wishlist.htm',
'whatsnew06.htm'
];
var list_current0 = [
'glossary.htm#AD Function',
'glossary.htm#AD of Base',
'glossary.htm#AD Type Sequence',
'glossary.htm#Base Function',
'glossary.htm#Base Type',
'glossary.htm#Dependent Variable',
'glossary.htm#Elementary Vector',
'glossary.htm#Independent Variable',
'glossary.htm#Operation',
'glossary.htm#Operation.Atomic',
'glossary.htm#Operation.Sequence',
'glossary.htm#Operation.Dependent',
'glossary.htm#Operation.Independent',
'glossary.htm#Parameter',
'glossary.htm#Sparsity Pattern',
'glossary.htm#Tape State',
'glossary.htm#Tape State.Empty',
'glossary.htm#Tape State.Recording',
'glossary.htm#Taylor Coefficient',
'glossary.htm#Variable'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
