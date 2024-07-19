using UnityEngine;

public class FloatingObject : MonoBehaviour
{
    public float amplitude = 0.1f; // 浮遊の振幅
    public float speed = 1.0f;     // 浮遊の速度

    private Vector3 startPos;      // 初期位置

    void Start()
    {
        startPos = transform.position; // 初期位置を保存
    }

    void Update()
    {
        // オブジェクトを浮遊させる
        float floatY = startPos.y + amplitude * Mathf.Sin(speed * Time.time);
        transform.position = new Vector3(transform.position.x, floatY, transform.position.z);

        // オブジェクトをランダムに回転させる（任意）
        transform.Rotate(Vector3.up, Time.deltaTime * 50.0f, Space.World);
    }
}
