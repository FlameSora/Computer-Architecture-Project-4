		.data
vec1:	.word	23
	.word	40
	.word	90
	.word	15
	.word	69
	.word	54
	.word	74
	.word	51
	.word	63
	.word	46
	.word	72
	.word	89
	.word	25
	.word	11
	.word	91
	.word	53
	.word	78
	.word	46
	.word	64
	.word	78
	.word	29
	.word	64
	.word	14
	.word	79
	.word	77
	.word	79
	.word	53
	.word	12
	.word	21
	.word	80
	.word	88
	.word	70
	.word	85
	.word	80
	.word	81
	.word	36
	.word	22
	.word	46
	.word	47
	.word	84
	.word	96
	.word	37
	.word	62
	.word	92
	.word	54
	.word	47
	.word	55
	.word	29
	.word	91
	.word	60
	.word	48
	.word	71
	.word	44
	.word	37
	.word	27
	.word	41
	.word	53
	.word	34
	.word	63
	.word	50
	.word	91
	.word	51
	.word	33
	.word	56
	.word	92
	.word	40
	.word	45
	.word	46
	.word	33
	.word	71
	.word	44
	.word	36
	.word	27
	.word	16
	.word	97
	.word	76
	.word	16
	.word	33
	.word	49
	.word	66
	.word	77
	.word	64
	.word	49
	.word	51
	.word	22
	.word	53
	.word	65
	.word	19
	.word	60
	.word	65
	.word	97
	.word	20
	.word	73
	.word	42
	.word	38
	.word	60
	.word	42
	.word	60
	.word	86
	.word	78
	.word	35
vec2:	.word	56
	.word	93
	.word	75
	.word	82
	.word	78
	.word	45
	.word	13
	.word	14
	.word	38
	.word	88
	.word	38
	.word	59
	.word	79
	.word	43
	.word	83
	.word	49
	.word	67
	.word	52
	.word	75
	.word	25
	.word	14
	.word	80
	.word	13
	.word	40
	.word	42
	.word	85
	.word	69
	.word	75
	.word	14
	.word	79
	.word	21
	.word	63
	.word	61
	.word	61
	.word	78
	.word	29
	.word	36
	.word	98
	.word	97
	.word	58
	.word	57
	.word	90
	.word	65
	.word	86
	.word	75
	.word	41
	.word	40
	.word	42
	.word	68
	.word	86
	.word	71
	.word	69
	.word	89
	.word	22
	.word	79
	.word	36
	.word	91
	.word	49
	.word	40
	.word	50
	.word	33
	.word	86
	.word	64
	.word	22
	.word	66
	.word	34
	.word	53
	.word	13
	.word	94
	.word	18
	.word	91
	.word	36
	.word	45
	.word	59
	.word	87
	.word	91
	.word	22
	.word	62
	.word	37
	.word	98
	.word	36
	.word	58
	.word	77
	.word	98
	.word	18
	.word	83
	.word	48
	.word	26
	.word	85
	.word	55
	.word	62
	.word	10
	.word	93
	.word	36
	.word	27
	.word	44
	.word	91
	.word	32
	.word	48
	.word	66
	.word	33
result:	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
veclen:	.word	100
		.text
main:
	la	$1, vec1
	la	$2, vec2
	la	$10, result
	la	$3, veclen
	lw	$3, 0($3)
loop:	
	lw	$7, 0($1)
	lw	$8, 0($2)
	addu	$9, $7, $8
	sw	$9, 0($10)
	addiu	$1, $1, 4
	addiu	$2, $2, 4	
	addiu	$10, $10, 4
	addiu	$5, $5, 1	
	bne	$3, $5,	loop
end:
	addiu	$11, $11, 1
	addiu	$11, $11, 1
	addiu	$11, $11, 1
	addiu	$11, $11, 1

