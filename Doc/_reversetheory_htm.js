var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_down3 = [
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
var list_down2 = [
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
var list_down1 = [
'forwardtheory.htm',
'reversetheory.htm',
'reverseidentity.htm'
];
var list_down0 = [
'expreverse.htm',
'logreverse.htm',
'sqrtreverse.htm',
'sincosreverse.htm',
'atanreverse.htm',
'asinreverse.htm',
'acosreverse.htm'
];
var list_current0 = [
'reversetheory.htm#Taylor Notation',
'reversetheory.htm#Binary Operators',
'reversetheory.htm#Binary Operators.Addition',
'reversetheory.htm#Binary Operators.Subtraction',
'reversetheory.htm#Binary Operators.Multiplication',
'reversetheory.htm#Binary Operators.Division',
'reversetheory.htm#Standard Math Functions'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
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
