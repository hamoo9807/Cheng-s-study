func maxArea(height []int) int {
    left, right := 0, len(height)-1
    maxArea, tempShort := 0, 0 // tempShort 标记历史短板高度
	/*Short：短板*/

    for left < right {
        currentShort := min(height[left], height[right])
        // 仅当当前短板 > 历史短板时，才计算面积（减少无效乘法）
        if currentShort > tempShort {
            currentArea := (right - left) * currentShort
            if currentArea > maxArea {
                maxArea = currentArea
                tempShort = currentShort
            }
        }
        // 移动短板指针
        if height[left] < height[right] {
            left++
        } else {
            right--
        }
    }
    return maxArea
}

func min(a, b int) int {
    if a < b {
        return a
    }
    return b
}