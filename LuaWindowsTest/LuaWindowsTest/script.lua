
str = "Initilizing..."

function aFun(node)
	if node == 1 then
		sendFunction(messageId,1)
	else
		sendFunction(messageId,2)
	end
end

function changeStr(string)
	str = string
end

function getRandomStr()
	local stringt="Random str is:";
	repeat
		stringt =stringt .. string.char(math.random(26)+96)
	until math.random()<0.1
	return stringt;
end