#!/usr/bin/python3

def binary_search(arr, target):
    # initializing the left and right pointers
    left = 0
    right = len(arr) - 1

    # loop through the array until the left pointer is greater than the right pointer
    while left <= right:
        # find the middle index
        mid = (left + right) // 2

        # if the middle element is the target, return its index
        if arr[mid] == target:
            return mid

        # if the target is less than the middle element, move the right pointer to the left
        # of the middle element
        elif target < arr[mid]:
            right = mid - 1

        # if the target is greater than the middle element, move the left pointer to the right
        # of the middle element
        else:
            left = mid + 1

    # if the target is not found in the array, return -1
    return -1

def main():
    # test empty array
    assert binary_search([], 5) == -1

    # test array with one element
    assert binary_search([1], 1) == 0
    assert binary_search([1], 5) == -1

    # test array with multiple elements
    assert binary_search([1, 2, 3, 4, 5], 3) == 2
    assert binary_search([1, 2, 3, 4, 5], 4) == 3
    assert binary_search([1, 2, 3, 4, 5], 1) == 0
    assert binary_search([1, 2, 3, 4, 5], 5) == 4

    # test array with multiple elements where the target is not in the array
    assert binary_search([1, 2, 3, 4, 5], 0) == -1
    assert binary_search([1, 2, 3, 4, 5], 6) == -1

main()

