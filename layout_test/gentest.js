window.onload = function() {
	printTest(
		new CxxEmitter(),
		document.getElementById('container')
	);	
}

function printTest (e, container) {
	e.push([
		' // Copyright 2017 The Lynx Authors. All rights reserved.',
	]);

	e.emitPrologue();
	var containerLayoutTree = calculateTree(container);

	for (var i=0; i<containerLayoutTree.length; i++) {
		e.emitTestPrologue(containerLayoutTree[i].name);
		setUpTestTree(
			e,
			undefined,
			containerLayoutTree[i],
			'root',
			null
		) 
		e.push('');
		e.AddElementToBody();
		assertTestTree(e, containerLayoutTree[i], 'root', null);
		e.emitTestEpilogue();
	}

	e.emitEpilogue();
  e.print();
}

function setUpTestTree(e, parent, node, nodeName, parentName, index) {
	e.emitTestTreePrologue(nodeName);

	e.SetStyles(node.style, nodeName);
	if (parentName) {
		e.NodeInsertChild(parentName, nodeName, index);
	}

	for (var i=0; i<node.children.length; i++) {
		e.push('');
		var childName = nodeName + '_child' + i;
		setUpTestTree(
			e,
			node,
			node.children[i],
			childName,
			nodeName,
			i);
	}
}

function assertTestTree(e, node, nodeName, parentName) {

	// 标注下方便bug定位
	e.TestTarget(node.name);
	e.AssertEqual(node.left, e.NodeLayoutGetLeft(nodeName));
	e.AssertEqual(node.top, e.NodeLayoutGetTop(nodeName));
	e.AssertEqual(node.width, e.NodeLayoutGetWidth(nodeName));
	e.AssertEqual(node.height, e.NodeLayoutGetHeight(nodeName));

	for (var i=0; i<node.children.length; i++ ) {
		e.push('');
		var childName = nodeName + '_child' + i;
		assertTestTree(e, node.children[i], childName, nodeName);
	}
}

function calculateTree(root) {
	var rootLayout = [];

	for (var i=0; i<root.children.length; i++) {
		var child = root.children[i];
		rootLayout.push({
			name: child.id,
			tagName: child.tagName,
			left: child.offsetLeft,
			top: child.offsetTop, 
			width: child.offsetWidth,
			height: child.offsetHeight,
			children: calculateTree(child),
			style: getStyleMap(child.getAttribute('style')),
			rowStyle: child.getAttribute('style')
		})
	}

	return rootLayout;
}

function getStyleMap(styleString) {
	var styles = {};
	if (!styleString) {
		return styles;
	}
	var styleArray = styleString.split(';');
	for (var i=0; i<styleArray.length; i++) {
		var tmp = styleArray[i].split(':');
		var tmpKey = processStyleKey(tmp[0].trim());
		styles[tmpKey] = processStyleValue(tmp[1]);
	}
	return styles;
}

function getSupportStyle (node) {
	return [
		'display',
		'position',
		'left',
		'right',
		'top',
		'bottom',
		'width',
		'height',
		'min-width',
		'min-height',
		'margin-left',
		'margin-right',
		'margin-top',
		'marigin-bottom',
		'padding-left',
		'padding-right',
		'padding-top',
		'padding-bottom',
		'border-width',
		'flex',
		'flex-direction',
		'flex-wrap',
		'justify-content',
		'align-content',
		'align-self',
		'align-item'
	].reduce(function(map, key){
			if (key in node.style) {
				map[key] = node.style[key];
			}
			return map;
	}, {});
}

function processStyleKey(value) {
	if (!value || value == undefined) {
		return ;
	}
	if (value.indexOf('-')>-1) {
		var keySplit = value.split('-');
		// 将第二个单词转为驼峰
		var secWord = keySplit[1];
		var upperSecWord = upperFirstWord(secWord);
		return keySplit[0] + upperSecWord;
	} else {
		return value;
	}
}

function processStyleValue(value) {
	if (!value || value == undefined) {
		return ;
	}
	return value.replace(/(^\s*)|(\s*$)/g, ""); 	
}

function upperFirstWord(word) {
	return word[0].toUpperCase() + word.slice(1);
}

var Emitter = function(lang, indent) {
	this.lang = lang;
	this.indent = indent;
	this.indents = [];
	this.lines = [];
};

Emitter.prototype = Object.create(Object.prototype, {
	constructor: { value: Emitter },

  pushIndent: {
		value: function() {
			this.indents.push(this.indent);
		}
  },

	popIndent : {
		value: function() {
			this.indents.pop(this.indent);
		}
	},

	push: {
		value: function(line) {
			if (line instanceof Array) {
				line.forEach(function(element) {
					this.push(element);
				}, this);
				return ;
			} else if (line.length > 0){
				line = this.indents.join('') + line;
			}
			this.lines.push(line);
		}
	},

	print: {
		value: function() {
			console.log(this.lines.join('\n'));
		}
	}
});